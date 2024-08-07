#include <iostream>
#include "../include/gstore/PathQueryHandler.h"

using namespace std;

/**
 * @brief 双向广度优先搜索算法
 *
 * 使用双向广度优先搜索算法查找从起点s到终点t的路径。
 *
 * @param s 起点
 * @param t 终点
 * @param directed 是否为有向图
 * @param pred_set 边的谓词集合
 * @param queryUtil 查询工具类指针
 * @param path_set 存储路径的向量引用
 */
void d_bfs(int s, int t, bool directed, std::vector<int> pred_set, PathQueryHandler *queryUtil, vector<int> &path_set)
{
    queue<int> q1, q2;
    while (!q1.empty())
        q1.pop();
    while (!q2.empty())
        q2.pop();
    if (s == t)
    {
        path_set.clear();
        path_set.push_back(s);
    }

    unordered_map<int, int> d, pe1, pv1, pe2, pv2;

    d[s] = 1;
    d[t] = 2;
    q1.push(s);
    q2.push(t);

    int h1, h2, hid;

    while (!q1.empty() && !q2.empty())
    {
        if (q1.size() < q2.size())
        {
            int u = q1.front();
            q1.pop();
            for (int pred : pred_set)
            {
                int num = queryUtil->getOutSize(u, pred);
                for (int k = num - 1; k >= 0; --k)
                {
                    int v = queryUtil->getOutVertID(u, pred, k);
                    if (!d[v])
                    {
                        d[v] = 1;
                        pe1[v] = pred;
                        pv1[v] = u;
                        q1.push(v);
                    }
                    else
                    {
                        if (d[v] == 2)
                        {
                            h1 = u;
                            h2 = v;
                            hid = pred;
                            goto label;
                        }
                    }
                }
            }
        }
        else
        {
            int u = q2.front();
            q2.pop();
            for (int pred : pred_set)
            {
                int num = queryUtil->getInSize(u, pred);
                for (int k = 0; k < num; ++k)
                {
                    int v = queryUtil->getInVertID(u, pred, k);
                    if (!d[v])
                    {
                        d[v] = 2;
                        pe2[v] = pred;
                        pv2[v] = u;
                        q2.push(v);
                    }
                    else
                    {
                        if (d[v] == 1)
                        {
                            h1 = v;
                            h2 = u;
                            hid = pred;
                            goto label;
                        }
                    }
                }
            }
        }
    }

label:;

    vector<int> Ans;
    Ans.clear();
    path_set.clear();

    int u = h1;
    while (1)
    {
        Ans.push_back(u);
        if (u == s)
            break;
        Ans.push_back(pe1[u]);
        u = pv1[u];
    }

    int sz = Ans.size();
    for (int i = sz - 1; i >= 0; --i)
        path_set.push_back(Ans[i]);

    path_set.push_back(hid);

    u = h2;
    while (1)
    {
        path_set.push_back(u);
        if (u == t)
            break;
        path_set.push_back(pe2[u]);
        u = pv2[u];
    }
}

/**
 * @description: 查询两个节点之间的路径
 * @param {vector<int>} iri_set 起始节点和终止节点
 * @param {bool} directed 是否有向图
 * @param {int} k 路径长度
 * @param {vector<int>} pred_set 路径上的谓词集合
 * @return {string} 路径字符串
 */
extern "C" string cyclePath(std::vector<int> iri_set, bool directed, int k, std::vector<int> pred_set, PathQueryHandler *queryUtil)
{
    int s = iri_set[0], t = iri_set[1];
    vector<int> path, path1, path2;
    if (directed)
    {
        d_bfs(s, t, directed, pred_set, queryUtil, path1);
        path1.pop_back();
        d_bfs(t, s, directed, pred_set, queryUtil, path2);
    }
    else
    {
        d_bfs(s, t, directed, pred_set, queryUtil, path1);
        int sz = path1.size();
        for (int i = sz - 1; i >= 0; --i)
            path2.push_back(path1[i]);
        path1.pop_back();
    }
    path.clear();
    for (int x : path1)
        path.push_back(x);
    for (int x : path2)
        path.push_back(x);
    return queryUtil->getPathString(path);
}

int main(int argc, char *argv[])
{
    PathQueryHandler queryUtil;
    // queryUtil.printCSR();
    // 数据集参数详见README.md
    std::vector<int> iri_set = {1, 3}; // <Bob> <Carol>
    std::vector<int> pred_set = {1}; // <喜欢>
    // 调用自定义函数，各参数定义详见操作手册.pdf
    string rt = cyclePath(iri_set, true, -1, pred_set, &queryUtil);
    std::cout << rt << std::endl;
}