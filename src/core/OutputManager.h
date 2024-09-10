#pragma once
#include "OutputOptions.h"
#include <string>
/*! \brief generates different formats of output according to the given statistics and options
*/
class OutputManager
{
  public:
    /*! \brief output statistics data into formats chosen by given options
    \param[in] opts output options
    */
    static void output(const OutputOptions &opts);
    /*! \brief output json file
    \param[in] filepath the output dir path
    \param[in] opts output options
    */
    static void output_json(const std::string &filepath, const OutputOptions &opts);
    /*! \brief output html file
    \param[in] filepath the output dir path
    \param[in] opts output options
    */
    static void output_html(const std::string &filepath, const OutputOptions &opts);
    /*! \brief output markdown file
    \param[in] filepath the output dir path
    \param[in] opts output options
    */
    static void output_md(const std::string &filepath, const OutputOptions &opts);
    /*! \brief output text format into stream
    \param[in] stream the output stream
    \param[in] opts output options
    */
    static void append_text(std::ostream &stream, const OutputOptions &opts);
    /*! \brief output text file
    \param[in] filepath the output dir path
    \param[in] opts output options
    */
    static void output_text(const std::string &filepath, const OutputOptions &opts);
    /*! \brief output data onto command console
    \param[in] filepath the output dir path
    \param[in] opts output options
    */
    static void output_console(const OutputOptions &opts);
};