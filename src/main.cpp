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
int getMarket(long idMarket, _ns1__marketResponse *res);
int saveSolutionTest(_ns1__saveSolutionRequest *req, string *res);
void printMarket(_ns1__marketResponse *res);
int idMarket = 421;
int main() {

// get a et market by idmarket
	_ns1__marketResponse *res = new _ns1__marketResponse;
	int returnCode = getMarket(idMarket, res);
	if (returnCode == SOAP_OK)
	{
		printMarket(res);
	}

	//save solution test
	_ns1__saveSolutionRequest *solutionReq = new _ns1__saveSolutionRequest;
	string *solutionRes = new string;
	saveSolutionTest(solutionReq, solutionRes);
	cout << "save solution... " << *solutionRes<<endl;

	return 0;
}

int getMarket(long idMarket, _ns1__marketResponse *res) {
	_ns1__marketRequest *req = new _ns1__marketRequest;
	req->idMarket = idMarket;
	if (etSoap11Proxy.market(req, res) == SOAP_OK)
	{
		return SOAP_OK;
	} else {
		etSoap11Proxy.soap_stream_fault(cerr);
		return etSoap11Proxy.error;
	}
}

int saveSolutionTest(_ns1__saveSolutionRequest *req, string *res)

{

	//market solution
	_ns1__marketSolution ms;
	ms.idMarket = idMarket;
	ms.mrgCost = 237;
	ms.profit = 127;

	_ns1__solution solution;
	solution.marketSolution = &ms;

	//solution of Australia
	_ns1__partySolution au;

	au.idMarket = idMarket;
	au.idParty = 21;
	au.em = 257.4;
	au.mrgEm = 237;
	au.mrgBase = 0;
	au.mrgTarget = 211.4;
	au.costBase = 0;
	au.costEm = 13914.7;
	au.costTarget = 12439.6;
	au.profit = 84.1;

	//solution of Canada
	_ns1__partySolution ca;

	ca.idMarket = idMarket;
	ca.idParty = 22;
	ca.em = 439.6;
	ca.mrgEm = 237;
	ca.mrgBase = 0;
	ca.mrgTarget = 250;
	ca.costBase = 0;
	ca.costEm = 17560.8;
	ca.costTarget = 19162.4;
	ca.profit = 42.9;

	solution.partySolution.push_back(&au);
	solution.partySolution.push_back(&ca);
	//solution.partySolution.insert(solution.partySolution.begin(),&au);
	//solution.partySolution.insert(solution.partySolution.begin(),&ca);

	_ns1__msg msg;
	msg.idMarket = idMarket;
	msg.msgType = 100;
	msg.msg = "Optimal solution";

	_ns1__msg warnMsg;
	warnMsg.idMarket = idMarket;
	warnMsg.msgType = 101;
	warnMsg.msg = "Test WARNING message";

	_ns1__msg errMsg;
	errMsg.idMarket = idMarket;
	errMsg.msgType = 102;
	errMsg.msg = "Test ERROR message";

	solution.msg.push_back(&msg);
	solution.msg.push_back(&warnMsg);
	solution.msg.push_back(&errMsg);
	// cout<<" party size " <<  solution.partySolution.size() <<endl;

	req->solution = &solution;
	//close the market
	req->solution->status = 14;
	//optimal solution
	req->solution->solverStatus = 210;

	if (etSoap11Proxy.saveSolution(req, *res) == SOAP_OK)
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
		cout << "party id: " << p->idParty << ", party name: " << p->name
				<< ", min em: " << p->minEm << ", base em: " << p->baseEm
				<< ", target em: " << p->targetEm << endl;
		_ns1__func *func = p->func;
		for (unsigned short j = 0; j < func->point.size(); j++) {
			_ns1__point *pt = func->point.at(j);
			cout << "em: " << pt->em << ", cost: " << pt->cost << endl;
		}

	}

}

