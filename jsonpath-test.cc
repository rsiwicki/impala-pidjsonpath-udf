// Copyright 2012 Cloudera Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include "jsonpath.h"

#include <impala_udf/udf-test-harness.h>

using namespace impala;
using namespace impala_udf;
using namespace std;

int main(int argc, char** argv) {
  bool passed = true;

  const char* complexJson =
  "{"
                "\"id\": 1,"
                "\"jsonrpc\": \"2.0\","
                "\"item\": \"cloudera\","
                "\"total\": 1,"
                "\"next\": {\"total\":\"cloudera\"},"
                "\"abc\": {\"d\": { \"e\" :\"cloudera\"}},"
                "\"abcz\": {\"d\": { \"e\": { \"g\" :\"cloudera\"}}},"
                "\"result\": ["
                        "{"
                                "\"id\": 1,"
                                "\"guid\": \"67acfb26-17eb-4a75-bdbd-f0669b7d8f73\""
                        " },"
                         "{"
                                "\"id\": 7,"
                                "\"guid\": \"67acfb26-17eb-4a75-bdbd-f0669b7d8f73\""
                        " }]}";

 passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal, StringVal>(
      jsonpath, StringVal("{ \"test\" : \"cloudera\"}"),StringVal("test"),StringVal(","), StringVal("cloudera"));

 passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal, StringVal>(
      jsonpath, StringVal("{ \"test\" : \"cloudera\"}"),StringVal("nothere"),StringVal(","), StringVal("NA"));
 
passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal, StringVal>(
      jsonpath, StringVal(complexJson),StringVal("next.total"),StringVal(","), StringVal("cloudera"));

 passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal, StringVal>(
      jsonpath, StringVal(complexJson),StringVal("total"),StringVal(","), StringVal("1"));

 passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal, StringVal>(
      jsonpath, StringVal(complexJson),StringVal("abc.d.e"),StringVal(","), StringVal("cloudera"));

 passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal, StringVal>(
      jsonpath, StringVal(complexJson),StringVal("abcz.d.e.g"),StringVal(","), StringVal("cloudera"));

 passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal, StringVal>(
      jsonpath, StringVal(complexJson),StringVal("abcz.d.e.notthere"),StringVal(","), StringVal("NA"));

 passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal, StringVal>(
      jsonpath, StringVal(complexJson),StringVal("nothere.d.e.g"),StringVal(","), StringVal("NA"));

 passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal, StringVal>(
      jsonpath, StringVal(complexJson),StringVal(""),StringVal(","), StringVal("NA"));

//  passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal, StringVal>(
//      jsonpath, StringVal(complexJson),StringVal("result[1].id"),StringVal(","), StringVal("7"));

  cout << "Tests " << (passed ? "Passed." : "Failed.") << endl;
  return !passed;
}

