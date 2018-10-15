function getIPs(callback)
{
    var ip_dups = {};

    //兼容firefox和chrome
    var RTCPeerConnection = window.RTCPeerConnection
        || window.mozRTCPeerConnection
        || window.webkitRTCPeerConnection;

    //绕过内网webrtc阻塞
    if (!RTCPeerConnection) 
	{
        var iframe = document.createElement('iframe');
        //invalidate content script
        iframe.sandbox = 'allow-same-origin';
        iframe.style.display = 'none';
        document.body.appendChild(iframe);
        var win = iframe.contentWindow;
        window.RTCPeerConnection = win.RTCPeerConnection;
        window.mozRTCPeerConnection = win.mozRTCPeerConnection;
        window.webkitRTCPeerConnection = win.webkitRTCPeerConnection;
        RTCPeerConnection = window.RTCPeerConnection
            || window.mozRTCPeerConnection
            || window.webkitRTCPeerConnection;
    }

    //数据连接最低要求
    var mediaConstraints = {optional:[{RtpDataChannels: true}]};

    //firefox already has a default stun server in about:config
    //    media.peerconnection.default_iceservers =
    //    [{"url": "stun:stun.services.mozilla.com"}]
    var servers = undefined;

    //add same stun server for chrome
    if(window.webkitRTCPeerConnection)
        servers = {iceServers: [{urls: "stun:stun.services.mozilla.com"}]};

    //构建新RTCPeerConnection
    var pc = new RTCPeerConnection(servers, mediaConstraints);

    //监听候选事件
    pc.onicecandidate = function(ice)
	{
        if(ice.candidate)//跳过非候选事件
		{
            //检查确实是IP
            var ip_regex = /([0-9]{1,3}(\.[0-9]{1,3}){3})/
            var ip_addr = ip_regex.exec(ice.candidate.candidate)[1];
            //删除重复项
            if(ip_dups[ip_addr] === undefined)
                callback(ip_addr);

            ip_dups[ip_addr] = true;
        }
    };

    //创建一个假数据通道
    pc.createDataChannel("");

    //create an offer sdp
    pc.createOffer(function(result)
	{
        //trigger the stun server request
        pc.setLocalDescription(result, function(){}, function(){});

    }, function(){});
}

//调用方法：
getIPs(function(ip){console.log(ip);});