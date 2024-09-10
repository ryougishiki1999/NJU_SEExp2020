#pragma once
#include "../AnalyzerOptions.h"
#include "../OutputOptions.h"
#include "../ResultCollector.h"
#include "JsonProc.h"
#include <string>
// helper functions to transform diagnostics and stats between text and json
namespace Json
{
/*! \brief parse the diagnostic into json tree
\param[in] diag the diagnostic
\param[in] opts ouput options
\return generated json value
*/
Value &parse(const OutputOptions &opts);
/*! \brief translate diagnostic data into string
\param[in] diag the diagnostic data
\param[in] opts output options
\return translated string
*/
std::string stringify(const OutputOptions &opts);
} // namespace Json