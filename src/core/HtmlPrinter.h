#ifndef CORE_HTML_PRINTER_H
#define CORE_HTML_PRINTER_H

#include "Logger.h"
#include "Statistics/StatisticsHelper.h"
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#define FILEMODE (S_IRWXU | S_IRWXG | S_IRWXO)
#ifdef __APPLE__
#include <sys/uio.h>
#else
#include <sys/io.h>
#endif
#endif
/*! \brief Html Printer
    
    the class to output html analysis result.
*/
class HtmlPrinter
{
  public:
    /*! \brief init html printer options
      \param[in] output_path the output path of html files and other files
      \param[in] analy_time the output option of analyzing time
      \param[in] bug_type the output option of bug type
      \param[in] loc the output option of loaction
      \param[in] checker the output option of checker name
      \param[in] note the output option of bug description
      \param[in] statis the output option of statistics result
  */
    HtmlPrinter(std::string output_path, bool analy_time = true, bool bug_type = true, bool loc = true,
                bool checker = true, bool note = true, bool statis = true);
    /*! \brief Interface for external calls for output html analysis result
    */
    void print();
    /*! \brief generate style.css file
     */
    void generate_style_css();
    /*! \brief generate stats.css file
     */
    void generate_stats_css();
    /*! \brief generate prettyprint.css file
     */
    void generate_prettyprint_css();
    /*! \brief generate prettyprint.js file
     */
    void generate_prettyprint_js();
    /*! \brief generate index.html file
     */
    void generate_index_html();
    /*! \brief generate stats.html file
     */
    void generate_stats_html();
    /*! \brief generate detail.html file
        \param[in] filename the string of the file's absolute path
     */
    void generate_detail_html(std::string filename);

  private:
    std::string output_path;//! the output path of output files

  public:
    bool analy_time;//! the output option of analyzing time
    bool bug_type;//! the output option of bug type
    bool loc;//! the output option of location
    bool checker;//! the output option of checker
    bool note;//! the output option of bug description
    bool statis;//the output option of statistics result
};

#endif // !CORE_HTML_PRINTER_H
