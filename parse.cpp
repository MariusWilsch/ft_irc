/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahammout <ahammout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:33:05 by ahammout          #+#    #+#             */
/*   Updated: 2023/09/23 16:10:08 by ahammout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::map;
using std::set;
using std::vector;
using std::cout;
using std::endl;

bool    whiteCheck(string str)
{
    for (unsigned int i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
            return false;
    }
    return true;
}

size_t  advParam(string param){
    int c = 0;
    for (unsigned int i = 0; i < param.size(); i++){
        if (param[i] == ',')
            c++;
    }
    return (c);
}

int splitParams(vector<string> &v){
    if (v.size() == 1 && v[0].compare("0") == 0){
        return (0);
    }
    if (v.size() < 1 || whiteCheck(v[0]))
        return (false);
    for (unsigned int i = 0; i < v.size(); i++)
    {
        string param = v[i];
        param.erase(std::remove(param.begin(), param.end(), ' '), param.end());
        unsigned int c = advParam(param);
        if (c == 0){
            if (param[0] == '#' || param[0] == '&' || param[0] == '+' || param[0] == '!'){
                param.erase(0, 1);
                cout << "Single PARAM NAME: " << param << endl;
            }
            else{
                cout << "Single PARAM KEY : " << param << endl;
            }
        }
        if (c >= 1)
        {
            for (unsigned int i = 0; i <= c; i++){
                unsigned int b = 0;
                unsigned int  e = param.find(',');
                string sub = param.substr(b, e);
                sub.erase(std::remove(sub.begin(), sub.end(), ' '), sub.end());
                param = param.substr(e + 1);
                if (sub[0] == '#' || sub[i] == '&' || sub[0] == '+' || sub[0] == '!'){
                    sub.erase(0, 1);
                    cout << "Splited PARAM NAME: " << sub << endl;
                }
                else{
                    cout << "Splited PARAM KEY: " << sub << endl;
                }
            }
        }
    }
    return (1);
}

int main(void){
    vector<string> v;
    v.push_back("#c,   #a,    #b"); 
    v.push_back("#d");
    v.push_back("K1, K2");
    v.push_back("K3");
    
    size_t r = splitParams(v);
    std::cout << "Function return: " << r << endl;
    return (0);
}