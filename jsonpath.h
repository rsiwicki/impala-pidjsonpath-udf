#include <impala_udf/udf.h>

using namespace impala_udf;

StringVal jsonpath(FunctionContext* context, const StringVal& input, const StringVal& query, const StringVal& delimiter);
