import networkx as nx
import pylab as plt

g = nx.Graph()

fixed_pos = {1: (1, 1), 2: (0.7, 2.2), 3: (0, 1.8), 4: (1.6, 2.3), 5: (2, 0.8), 6: (-0.6, -0.6), 7: (-1.3, 0.8),
             8: (-1.5, -1), 9: (0.5, -1.5), 10: (1.7, -0.8), 11: (1.5, -2.3)}  # set fixed layout location

# pos=nx.spring_layout(g) # or you can use other layout set in the module
# 实际画节点的代码，从fixed_pos中指定一部分来画
nx.draw_networkx_nodes(g, pos=fixed_pos, nodelist=[1, 2, 3, 4, 5],
                       node_color='g', node_size=600)
# 画连线的diamagnetic，同上
nx.draw_networkx_edges(g, pos=fixed_pos, edgelist=[(1, 2), (1, 3), (1, 4), (1, 5), (1, 9)], edge_color='g',
                       width=[4.0, 4.0, 4.0, 4.0, 4.0], label=[1, 2, 3, 4, 5], node_size=600)

nx.draw_networkx_nodes(g, pos=fixed_pos, nodelist=[6, 7, 8],
                       node_color='r', node_size=600)
nx.draw_networkx_edges(g, pos=fixed_pos, edgelist=[(6, 7), (6, 8), (1, 6)], width=[4.0, 4.0, 4.0], edge_color='r',
                       node_size=600)

nx.draw_networkx_nodes(g, pos=fixed_pos, nodelist=[9, 10, 11],
                       node_color='b', node_size=600)
nx.draw_networkx_edges(g, pos=fixed_pos, edgelist=[(6, 9), (9, 10), (9, 11)], width=[4.0, 4.0, 4.0], edge_color='b',
                       node_size=600)

# 在点旁边添加文字
plt.text(fixed_pos[1][0], fixed_pos[1][1] + 0.2, s='1', fontsize=40)
plt.text(fixed_pos[2][0], fixed_pos[2][1] + 0.2, s='2', fontsize=40)
plt.text(fixed_pos[3][0], fixed_pos[3][1] + 0.2, s='3', fontsize=40)
plt.text(fixed_pos[4][0], fixed_pos[4][1] + 0.2, s='4', fontsize=40)
plt.text(fixed_pos[5][0], fixed_pos[5][1] + 0.2, s='5', fontsize=40)
plt.text(fixed_pos[6][0], fixed_pos[6][1] + 0.2, s='6', fontsize=40)
plt.text(fixed_pos[7][0], fixed_pos[7][1] + 0.2, s='7', fontsize=40)
plt.text(fixed_pos[8][0], fixed_pos[8][1] + 0.2, s='8', fontsize=40)
plt.text(fixed_pos[9][0], fixed_pos[9][1] + 0.2, s='9', fontsize=40)
plt.text(fixed_pos[10][0], fixed_pos[10][1] + 0.2, s='10', fontsize=40)
plt.text(fixed_pos[11][0], fixed_pos[11][1] + 0.2, s='11', fontsize=40)

axes = plt.gca() # get current axes
axes.grid(True)  # 添加网格
plt.show()