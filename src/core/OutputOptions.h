#ifndef CORE_OUTPUT_OPTIONS_H
#define CORE_OUTPUT_OPTIONS_H

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
/*! \brief Output Options

    This class stores all the options user can customize to control the output manager.
*/
class OutputOptions
{
  public:
    OutputOptions();
    /*! \brief dump the options to the output stream
        \param[in] os output stream
    */
    void dump(std::ostream &os);
    /*! \brief reset the options to default values.
     */
    void reset();
    /*! \brief add a new output format
        \param[in] f output format you want to add
    */
    void add_output_format(const std::string &f);
    /*! \brief clear the output formats
     */
    void clear_output_format();

  public:
    std::uint32_t output_time : 1;           //! output time info in the report
    std::uint32_t output_checker : 1;        //! output checker info in the report
    std::uint32_t output_description : 1;    //! output diagnostic description in the report
    std::uint32_t output_stats : 1;          //! output stats in the report
    std::string output_path = "";            //! the path of the report
    std::vector<std::string> output_formats; //! report's formats
};
#endif