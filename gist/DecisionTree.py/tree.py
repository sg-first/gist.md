import sklearn.tree as st
import process_data_func as process_data
import model_1_func as m1
import pickle
import bayes_opt

def f1(y_true, y_pred):
    l=len(y_true)

    def recall(y_true, y_pred):
        tot=0
        num=0
        for i in range(l):
            if y_true[i]==1:
                tot+=1
                if y_pred[i]==1:
                    num+=1
        return num/tot

    def precision(y_true, y_pred):
        tot = 0
        num = 0
        for i in range(l):
            if y_pred[i] == 1:
                tot += 1
                if y_true[i] == 1:
                    num += 1
        if tot==0:
            return 0
        else:
            return num / tot

    precision = precision(y_true, y_pred)
    recall = recall(y_true, y_pred)
    s=precision+recall
    if s==0:
        return 0
    else:
        return 2 * ((precision * recall) / (precision + recall))

model=None

def genTree(prop,depth):
    process_data.genData(process_data.posNum*prop)
    data,label,data_test,label_test=m1.getData()

    global model
    # 基于普通决策树建模-->训练模型-->测试模型-----单棵决策树
    model = st.DecisionTreeClassifier(max_depth=depth)
    model.fit(data, label)

    pred_test_y = model.predict(data_test)
    pred_data=model.predict(data)

    valF1=f1(label_test,pred_test_y)
    print(valF1)
    print(f1(label,pred_data))
    return valF1

rf_bo = bayes_opt.BayesianOptimization(
        genTree,
        {
            'prop': (1,20),
            'depth': (1,55),
        }
    )
rf_bo.maximize()

f=open('tree.pkl','wb')
pickle.dump(model,f)

def draw(model):
    dot_data = st.export_graphviz(
        model,
        out_file=None,
        feature_names=['prov_id', 'area_id', 'chnl_type', 'service_type', 'product_type',
                                                         'innet_months', 'total_times', 'total_flux', 'total_fee',
                                                         'pay_fee', 'sex', 'age', 'manu_name', 'term_type', 'max_rat_flag',
                                                         'is_5g_base_cover', 'is_work_5g_cover', 'is_home_5g_cover',
                                                         'is_work_5g_cover_l01', 'is_home_5g_cover_l01',
                                                         'is_work_5g_cover_l02', 'is_home_5g_cover_l02', 'activity_type',
                                                         'is_act_expire', 'comp_type', 'call_days', 're_call10',
                                                         'short_call10', 'long_call10', 'bank_cnt', 'game_app_flux',
                                                         'live_app_flux', 'video_app_flux', 'city_5g_ratio', 'city_level',
                                                         'app_sum'],
        filled=True,
        impurity=False,
        rounded=True
    )

    import pydotplus
    graph = pydotplus.graph_from_dot_data(dot_data)
    graph.get_nodes()[7].set_fillcolor("#FFF2DD")
    graph.write_png("out.png")  # 当前文件夹生成out.png
