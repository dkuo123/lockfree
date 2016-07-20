#include "NewTraderAlphas.h"
#include <guibase/rapidjson/rapidjson.h>
#include <guibase/rapidjson/filestream.h>  // wrapper of C stream for prettywriter as output
#include <guibase/rapidjson/document.h>
#include <fstream>
#include <regex>
#include <iostream>


using namespace Wake::Alpha;

namespace Wake {
namespace Traders {
namespace Prod {

using namespace std;
using namespace rapidjson;

namespace wu = Wake::Utilities;


NewTraderAlphas::NewTraderAlphas(): eventAlphas(LastAlphaEvent) {}

void NewTraderAlphas::loadAlphas(const string& cfg_file) {

  for (auto& i:eventAlphas) i.resize(t2cMap.size()+1);

  FILE * pFile = fopen (cfg_file.c_str(), "r");
  FileStream is(pFile);
  Document document;
  document.ParseStream<0>(is);
  const Value& alphas = document["Signals"];
  assert(alphas.IsArray());
  for(auto i = 0; i < alphas.Size(); ++i) {
    const Value& alpha = alphas[i];
    string sigName = alpha["Name"].GetString();
    if (alphas.find(sigName) !=  alphas.end())
      throw string("alpha signal ").append(sigName).append(" defined before");
    AlphaElement* alg = AlphaElementFactory::create(alpha["Class"].GetString());
    alg->init(InitValue()); // will set all the init values for this alpha
    alphas[sigName] = unique_ptr<AlphaElement>(alg);
    if (alpha.HasMember("OnBook"))
      if (alpha["OnBook"].IsInt())
        eventAlphas[OnBook][t2cMap[alpha["OnBook"].GetInt()+1]].push_back(alg);
      else if (alpha["OnBook"].IsArray())
        for(auto j = 0; j < alpha["OnBook"].Size(); ++j)
          eventAlphas[OnBook][t2cMap[alpha["OnBook"][j].GetInt()+1]].push_back(alg);


  }


  wu::CSV<string> alphasCSV(cfg_file);
  try {
    for (int i = 0; i < alphasCSV.getNRow(); ++i) {
      string sigName = alphasCSV.getValue(i,0);
      if (alphas.find(sigName) !=  alphas.end())
        throw string("alpha signal ").append(sigName).append(" defined before");
      AlphaElement* alg = AlphaElementFactory::create(alphasCSV.getValue(i,1));
      alg->init(InitValue());
      alphas[sigName] = unique_ptr<AlphaElement>(alg);
      if (!alphasCSV.getValue(i,2).empty()) {
        regex reg("(\\w+)");
        sregex_iterator it(alphasCSV.getValue(i,2).begin(), alphasCSV.getValue(i,2).end(), reg), it_end;
        while (it != it_end) {
          sigName = it->str();
          auto it2 = alphas.find(sigName);
          if (it2 == alphas.end())
            throw string("alpha signal ").append(sigName).append(" NOT defined before");
          alg->addChild(it2->second.get());
        }
        for(auto j = (int)OnBook; j < LastAlphaEvent; ++j) {
          string tenors = alphasCSV.getValue(i,3+j);
          if (tenors.empty())
            continue;
          else if (tenors == "*")
            eventAlphas[j][0].push_back(alg); // event[0] is for all tenors
          else {
            regex reg2("(\\d)+");
            sregex_iterator it2(tenors.begin(), tenors.end(), reg2), it2_end;
            while (it2 != it2_end) {
              cout << "tenor = " << it2->str() << endl;
              eventAlphas[j][t2cMap[atoi(it2->str().c_str())]+1].push_back(alg);
              ++it2;
            }

          }
        }
      }
    }
  }
  catch(regex_error& e) {
    cout << e.what() << endl;
  }
  catch(string& e) {
    cout << e << endl;
  }
}

string NewTraderAlphas::toString() const {
  return string("NewTraderAlphas");

}
}}};


