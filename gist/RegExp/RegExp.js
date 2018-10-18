function RegexCaptureExample()
{
	str = "a=100";
	reg = /(.*)=(.*)/gi
	var result;
	if ((result = reg.exec(str)) != null) 
	{
	    output(result[0]); //a=100, text matching pattern
	    output(result[1]); //a
	    output(result[2]); //100
	}
}

function RegexReplaceExample()
{
	var str="5=a";
	str=str.replace(/(.*)=/gi,"cbd=");
	output(str); //cbd=a
}

function RegexReplaceExample2()
{
	var str="5=a";
	str=str.replace(/(.*)=(.*)/gi,"$2=$1");
	output(str); //a=5
}