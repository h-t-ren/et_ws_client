/*
* test.cpp
*
* Created on: Jan 11, 2012
* Author: hongtao
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "soapetSoap11Proxy.h"
#include "etSoap11.nsmap"

using namespace std;
etSoap11Proxy etSoap11Proxy(SOAP_XML_INDENT);
int getMarket(long idMarket,_ns1__marketResponse *res);
void printMarket(_ns1__marketResponse *res);
int main() {

// get a et market by idmarket
_ns1__marketResponse *res =new _ns1__marketResponse;
int returnCode = getMarket(405,res);
if(returnCode==SOAP_OK)
{
  printMarket(res);
}

//todo: saveSolution.
return 0;
}


int getMarket(long idMarket,_ns1__marketResponse *res) {
  _ns1__marketRequest *req = new _ns1__marketRequest;
  req->idMarket=idMarket;
  if (etSoap11Proxy.market(req,res) == SOAP_OK)
   {
      return SOAP_OK;
   } else {
	  etSoap11Proxy.soap_stream_fault(cerr);
      return etSoap11Proxy.error;
   }
}

void printMarket(_ns1__marketResponse *res)

{
	cout << "Market id : " << res->market->idMarket << endl;

	cout << "Market name : " << res->market->name << endl;
	cout << "solver id: " << res->market->idSolver << endl;
	for (unsigned short i = 0; i < res->market->party.size(); i++) {
		_ns1__party *p = res->market->party.at(i);
                cout << "party id: " << p->idParty <<", party name: " << p->name << ", min em: " << p->minEm
                                << ", base em: " << p->baseEm << ", target em: "
                                << p->targetEm << endl;
		_ns1__func *func = p->func;
		for (unsigned short j = 0; j < func->point.size(); j++) {
			_ns1__point *pt = func->point.at(j);
			cout << "em: " << pt->em << ", cost: " << pt->cost << endl;
		}

	}

}



