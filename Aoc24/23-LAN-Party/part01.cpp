#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <tuple>
#include <map>
#include <queue>
#include <stack>
#include <array>
#include <unordered_map>
#include <set>
#include <string>
#include <vector>


using namespace std;


auto split(string s, string delimiter) -> vector<string>
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}


class Solution
{
private:
    fstream fin;
    set<string> computers;
    set<pair<string, string>> directional;
    map<string, vector<string>> connections;
    vector<string> maxCliques;

    void parse()
    {
        string line;
        while (getline(this->fin, line))
        {
            vector<string> parts = split(line, "-");
            this->computers.insert(parts[0]);
            this->computers.insert(parts[1]);
            this->connections[parts[0]].push_back(parts[1]);
            this->connections[parts[1]].push_back(parts[0]);
            this->directional.insert({parts[0], parts[1]});
            this->directional.insert({parts[1], parts[0]});
        }
    }

    void BronKerbosch(set<string> R, set<string> P, set<string> X)
    {
        if (P.empty() && X.empty())
        {
            if (R.size() > this->maxCliques.size())
                this->maxCliques = vector<string>(R.begin(), R.end());
            return;
        }

        string pivot = *P.begin();
        set<string> unionPX = P;
        unionPX.insert(X.begin(), X.end());

        for (auto &v : unionPX)
            if (this->connections[v].size() > this->connections[pivot].size())
                pivot = v;

        set<string> excludedNeighbors;
        for (auto &neighbor : this->connections[pivot])
            if (P.find(neighbor) != P.end())
                excludedNeighbors.insert(neighbor);

        vector<string> candidates;
        for (auto &v : P)
            if (excludedNeighbors.find(v) == excludedNeighbors.end())
                candidates.push_back(v);

        for (auto &v : candidates)
        {
            set<string> newR = R;
            newR.insert(v);

            set<string> newP, newX;

            for (auto &u : P)
                if (find(this->connections[v].begin(), this->connections[v].end(), u) != this->connections[v].end())
                    newP.insert(u);

            for (auto &u : X)
                if (find(this->connections[v].begin(), this->connections[v].end(), u) != this->connections[v].end())
                    newX.insert(u);

            BronKerbosch(newR, newP, newX);

            P.erase(v);
            X.insert(v);
        }
    }

    vector<string> findMaxClique()
    {
        set<string> R, P, X;

        for (auto c : this->computers)
            P.insert(c);

        BronKerbosch(R, P, X);

        return this->maxCliques;
    }

public:
    Solution(string fileName)
    {
        this->fin.open(fileName, ios::in);
        this->parse();
        this->fin.close();
    }

    auto part1() -> int
    {
        int sol = 0;
        set<tuple<string, string, string>> visited;
        for (auto c1 : this->computers)
        {
            for (auto c2 : this->computers)
            {
                if (c1 == c2)
                    continue;

                if (this->directional.find({c1, c2}) == this->directional.end())
                    continue;

                for (auto c3 : this->computers)
                {
                    if (c3 == c1 || c3 == c2)
                        continue;

                    if (c1[0] != 't' && c2[0] != 't' && c3[0] != 't')
                        continue;

                    if (
                        this->directional.find({c1, c2}) != this->directional.end() &&
                        this->directional.find({c2, c3}) != this->directional.end() &&
                        this->directional.find({c3, c1}) != this->directional.end() && visited.find({c1, c2, c3}) == visited.end())
                    {
                        visited.insert({c1, c2, c3});
                        visited.insert({c1, c3, c2});
                        visited.insert({c2, c1, c3});
                        visited.insert({c2, c3, c1});
                        visited.insert({c3, c1, c2});
                        visited.insert({c3, c2, c1});

                        ++sol;
                    }
                }
            }
        }
        return sol;
    };

    auto part2() -> string
    {

        string sol = "";

        vector<string> maxClique = this->findMaxClique();

        sort(maxClique.begin(), maxClique.end());

        for (int i = 0; i < maxClique.size(); i++)
        {
            sol += maxClique[i];
            if (i != maxClique.size() - 1)
                sol += ",";
        }

        return sol;
    }
};

auto main() -> int
{

    Solution aoc = Solution("input.txt");
    cout << "Part 1: " << aoc.part1() << endl;
    cout << "--------------------------" << endl;
    cout << "Part 2: " << aoc.part2() << endl;

    return 0;
}