#include "jsonpath.h"

#include <cctype>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <sstream>

#include "../rapidjson/include/rapidjson/rapidjson.h"
#include "../rapidjson/include/rapidjson/document.h"

using namespace std;

vector<string> split(const char *str, char c = ' ')
{
    	vector<string> result;

    	do
    	{
        	const char *begin = str;

        	while(*str != c && *str)
            		str++;

        	result.push_back(string(begin, str));
    	} while (0 != *str++);

    	return result;
}

template <typename T>
	T StringToNumber ( const string &Text )
  	{
     		istringstream ss(Text);
     		T result;
     		return ss >> result ? result : 0;
  	}

template <typename T>
	string NumberToString ( T Number )
  	{
     		ostringstream ss;
     		ss << Number;
     		return ss.str();
  	}

const char* convert(rapidjson::Value& value, const char* item) {

	if(value[item].IsString()) {
		return value[item].GetString();
	} else {
		return  NumberToString(value[item].GetInt()).c_str();
	}

}

StringVal jsonpath(FunctionContext* context, const StringVal& value, const StringVal& query, const StringVal& delimiter) {
  		
		if (value.is_null || query.is_null) return StringVal::null();

  		std::string json(reinterpret_cast<const char*>(value.ptr));
  		std::string jsonpath(reinterpret_cast<const char*>(query.ptr));

  		const char* jsonl = json.c_str();
  		const char* jsonpq = jsonpath.c_str();

		const char* returnthis = NULL;

  		vector<string> pathQuery = split(jsonpq,'.');

  		rapidjson::Document dx;
  		dx.Parse<0>(jsonl);

 		int numTokens = pathQuery.size();
 
  		if (numTokens==1) {
			if(!dx.HasMember(pathQuery[0].c_str())) { 
				returnthis = "NA";
     			} else {
     				returnthis = convert(dx, pathQuery[0].c_str());
     			}
  		} else {
  			if(!dx.HasMember(pathQuery[0].c_str())) { 
  				returnthis = "NA";
  			} else {
  				rapidjson::Value& seed = dx[pathQuery[0].c_str()];
 
  				for(int i = 1; i <= numTokens; i++) {
				const char* levelName = pathQuery[i].c_str();
	
        			if(!seed.HasMember(levelName)) {
					returnthis = "NA";
					break;
				} else {
					if(i == numTokens-1) {
						if(seed.HasMember(levelName)) {
							returnthis = seed[levelName].GetString();
						} else {
							returnthis = "NA";
						}
						break;
					} else { 
		    				rapidjson::Value& wseed = seed[levelName];
                    				seed = wseed;
					}
				}
			}
  		}
	}

	ostringstream outputstream("");

	outputstream << returnthis;

	string output = outputstream.str();

 	StringVal result(context, output.size());
 	memcpy(result.ptr, output.c_str(), output.size());

	return result;
}
