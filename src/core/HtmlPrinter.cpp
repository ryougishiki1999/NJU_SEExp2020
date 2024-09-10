#include "HtmlPrinter.h"

HtmlPrinter::HtmlPrinter(std::string output_path, bool analy_time, bool bug_type, bool loc, bool checker,
            bool note, bool statis)
{
    //init the output information options
    this->output_path = output_path;
    this->analy_time = analy_time;
    this->bug_type = bug_type;
    this->loc = loc;
    this->checker = checker;
    this->note = note;
    this->statis = statis;
    if (output_path.back() == '/')
    {
        output_path = output_path.substr(0, output_path.size() - 1);
    }
    else
    {
        output_path = output_path;
    }
    //Create the necessary folders
    //./css store css files(style.css,stats.css,prettyprint.css)
    std::string css_path = output_path + "/css";
    //./js store js file(prettyprint.js)
    std::string js_path = output_path + "/js";
    //./modules store detailed information html
    std::string module_path = output_path + "/modules";
    if (access(css_path.c_str(), 0) != 0)
    {
#ifdef _WIN32
        mkdir(css_path.c_str());
#else
        mkdir(css_path.c_str(), FILEMODE);
#endif
    }
    if (access(js_path.c_str(), 0) != 0)
    {
#ifdef _WIN32
        mkdir(js_path.c_str());
#else
        mkdir(js_path.c_str(), FILEMODE);
#endif
    }
    if (access(module_path.c_str(), 0) != 0)
    {
#ifdef _WIN32
        mkdir(module_path.c_str());
#else
        mkdir(module_path.c_str(), FILEMODE);
#endif
    }
}
// generate style css for index.html
void HtmlPrinter::generate_style_css()
{
    std::string out_filename = output_path + "/css/style.css";
    //create sytle.css file
    std::ofstream out(out_filename, std::ios::out | std::ios::trunc);
    if (!out.is_open())
    {
        INFO("Error when creating the style.css file\n");
        return;
    }
    //set table style
    out << "#rounded-corner" << std::endl;
    out << "{" << std::endl;
    out << "	font-family: \"Lucida Sans Unicode\", \"Lucida Grande\", "
           "Sans-Serif;"
        << std::endl;
    out << "	font-size: 12px;" << std::endl;
    out << "	margin: 45px;" << std::endl;
    out << "	width: 90%;" << std::endl;
    out << "  text-align: left;" << std::endl;
    out << "  border-collapse: collapse;" << std::endl;
    out << "  border-radius: 15px;" << std::endl;
    out << "  overflow: hidden;" << std::endl;
    out << "}" << std::endl;
    //set the top-left corner style of the table style
    out << "#rounded-corner thead th.rounded-left" << std::endl;
    out << "{" << std::endl;
    out << "  background: #b9c9fd;" << std::endl;
    out << "}" << std::endl;
    //set the top-right corner style of the table
    out << "#rounded-corner thead th.rounded-right" << std::endl;
    out << "{" << std::endl;
    out << "  background: #b9c9fd;" << std::endl;
    out << "}" << std::endl;
    //set the th style of the table style
    out << "#rounded-corner th" << std::endl;
    out << "{" << std::endl;
    out << "  padding: 8px;" << std::endl;
    out << "  font-weight: normal;" << std::endl;
    out << "  font-size: 13px;" << std::endl;
    out << "  color: #039;" << std::endl;
    out << "  background: #b9c9fd;" << std::endl;
    out << "}" << std::endl;
    //set the td style of the table
    out << "#rounded-corner td" << std::endl;
    out << "{" << std::endl;
    out << "  padding: 8px;" << std::endl;
    out << "  background: #e5e9f8;" << std::endl;
    out << "  border-top: 1px solid #fff;" << std::endl;
    out << "  color: #669;" << std::endl;
    out << "}" << std::endl;
    //set the bottom-left corner style of the table
    out << "#rounded-corner tfoot td.rounded-foot-left" << std::endl;
    out << "{" << std::endl;
    out << "  background: #e5e9f8;" << std::endl;
    out << "}" << std::endl;
    //set the bottom-right corner style of the table
    out << "#rounded-corner tfoot td.rounded-foot-right" << std::endl;
    out << "{" << std::endl;
    out << "  background: #e5e9f8;" << std::endl;
    out << "}" << std::endl;
    //set the td style when mousr move on it 
    out << "#rounded-corner tbody tr:hover td" << std::endl;
    out << "{" << std::endl;
    out << "  background: #cad4fc;" << std::endl;
    out << "}" << std::endl;
    //set the button wrapper div style
    out << "#button-wrapper" << std::endl;
    out << "{" << std::endl;
    out << "    margin-left: auto;" << std::endl;
    out << "    margin-right: auto;" << std::endl;
    out << "    margin-top: 20px;" << std::endl;
    out << "    margin-bottom: 20px;" << std::endl;
    out << "    padding-top: 5px;" << std::endl;
    out << "    padding-bottom: 5px;" << std::endl;
    out << "    padding-left: 0px;" << std::endl;
    out << "    padding-right: 0px;" << std::endl;
    out << "    text-align: center;" << std::endl;
    out << "    width: 200px;" << std::endl;
    out << "    background-color: #fff;" << std::endl;
    out << "    border-left: 0px;" << std::endl;
    out << "    border-right: 0px;" << std::endl;
    out << "    border-top: 0px;" << std::endl;
    out << "    border-bottom: 0px;" << std::endl;
    out << "}" << std::endl;
    //set the jump button style
    out << ".click-button" << std::endl;
    out << "{" << std::endl;
    out << "    border-width: 1px;" << std::endl;
    out << "    border-style: solid;" << std::endl;
    out << "    border-color: #eee;" << std::endl;
    out << "    background-color: #e5e9f8;" << std::endl;
    out << "    color:#038;" << std::endl;
    out << "    border-radius: 15px;" << std::endl;
    out << "    text-align: center;" << std::endl;
    out << "    text-decoration: none;" << std::endl;
    out << "    font-size: 16px;" << std::endl;
    out << "    cursor: pointer;" << std::endl;
    out << "    width: 200px;" << std::endl;
    out << "    height: 50px;" << std::endl;
    out << "}" << std::endl;
    //set the jump button style when mouse move on it
    out << ".click-button:hover" << std::endl;
    out << "{" << std::endl;
    out << "    background-color: #039;" << std::endl;
    out << "    color:white;" << std::endl;
    out << "}" << std::endl;
    out.close();
}
// generate statistic style for stats.html
void HtmlPrinter::generate_stats_css()
{
    //create stats.css file
    std::string out_filename = output_path + "/css/stats.css";
    std::ofstream out(out_filename, std::ios::out | std::ios::trunc);
    if (!out.is_open())
    {
        INFO("Error when creating the stats.css file\n");
        return;
    }
    //set the thanks info div style
    out << "#thanks-info" << std::endl;
    out << "{" << std::endl;
    out << "    margin-left: auto;" << std::endl;
    out << "    margin-right: auto;" << std::endl;
    out << "    margin-top: 20px;" << std::endl;
    out << "    margin-bottom: 20px;" << std::endl;
    out << "    padding-top: 5px;" << std::endl;
    out << "    padding-bottom: 5px;" << std::endl;
    out << "    padding-left: 0px;" << std::endl;
    out << "    padding-right: 0px;" << std::endl;
    out << "    text-align: center;" << std::endl;
    out << "    width: 1000px;" << std::endl;
    out << "    background-color: #fff;" << std::endl;
    out << "    border-left: 0px;" << std::endl;
    out << "    border-right: 0px;" << std::endl;
    out << "    border-top: 3px;" << std::endl;
    out << "    border-bottom: 3px;" << std::endl;
    out << "    border-style: solid;" << std::endl;
    out << "    border-color: #e5e9f8;" << std::endl;
    out << "}" << std::endl;
    //set the overall result info div style
    out << "#result-info" << std::endl;
    out << "{" << std::endl;
    out << "    margin-left: auto;" << std::endl;
    out << "    margin-right: auto;" << std::endl;
    out << "    margin-top: 20px;" << std::endl;
    out << "    margin-bottom: 20px;" << std::endl;
    out << "    padding-top: 5px;" << std::endl;
    out << "    padding-bottom: 5px;" << std::endl;
    out << "    padding-left: 10px;" << std::endl;
    out << "    padding-right: 10px;" << std::endl;
    out << "    text-align: left;" << std::endl;
    out << "    width: 1000px;" << std::endl;
    out << "    background-color: #e5e9f8;" << std::endl;
    out << "    border-left: 5px;" << std::endl;
    out << "    border-right: 5px;" << std::endl;
    out << "    border-top: 5px;" << std::endl;
    out << "    border-bottom: 5px;" << std::endl;
    out << "    border-style: outset;" << std::endl;
    out << "    border-color: #e5e9f8;" << std::endl;
    out << "}" << std::endl;
    //set table style
    out << "#rounded-corner" << std::endl;
    out << "{" << std::endl;
    out << "	font-family: \"Lucida Sans Unicode\", \"Lucida Grande\", "
           "Sans-Serif;"
        << std::endl;
    out << "	font-size: 12px;" << std::endl;
    out << "    margin-left: auto;" << std::endl;
    out << "    margin-right: auto;" << std::endl;
    out << "    margin-top: 40px;" << std::endl;
    out << "    margin-bottom: 40px;" << std::endl;
    out << "	width: 90%;" << std::endl;
    out << "    text-align: left;" << std::endl;
    out << "    border-collapse: collapse;" << std::endl;
    out << "    border-radius: 15px;" << std::endl;
    out << "    overflow: hidden;" << std::endl;
    out << "}" << std::endl;
    //set the top-left corner style of the table
    out << "#rounded-corner thead th.rounded-left" << std::endl;
    out << "{" << std::endl;
    out << "  background: #b9c9fd;" << std::endl;
    out << "}" << std::endl;
    // set the top-right corner style of the table
    out << "#rounded-corner thead th.rounded-right" << std::endl;
    out << "{" << std::endl;
    out << "  background: #b9c9fd;" << std::endl;
    out << "}" << std::endl;
    // set the th style of the table
    out << "#rounded-corner th" << std::endl;
    out << "{" << std::endl;
    out << "  padding: 8px;" << std::endl;
    out << "  font-weight: normal;" << std::endl;
    out << "  font-size: 13px;" << std::endl;
    out << "  color: #039;" << std::endl;
    out << "  background: #b9c9fd;" << std::endl;
    out << "}" << std::endl;
    // set the td style of the table
    out << "#rounded-corner td" << std::endl;
    out << "{" << std::endl;
    out << "  padding: 8px;" << std::endl;
    out << "  background: #e5e9f8;" << std::endl;
    out << "  border-top: 1px solid #fff;" << std::endl;
    out << "  color: #669;" << std::endl;
    out << "}" << std::endl;
    // set the bottom-left corner style of the table
    out << "#rounded-corner tfoot td.rounded-foot-left" << std::endl;
    out << "{" << std::endl;
    out << "  background: #e5e9f8;" << std::endl;
    out << "}" << std::endl;
    // set the bottom-right corner style of the table
    out << "#rounded-corner tfoot td.rounded-foot-right" << std::endl;
    out << "{" << std::endl;
    out << "  background: #e5e9f8;" << std::endl;
    out << "}" << std::endl;
    // set the td style when mousr move on it 
    out << "#rounded-corner tbody tr:hover td" << std::endl;
    out << "{" << std::endl;
    out << "  background: #cad4fc;" << std::endl;
    out << "}" << std::endl;
    // set the button wrapper div style
    out << "#button-wrapper" << std::endl;
    out << "{" << std::endl;
    out << "    margin-left: auto;" << std::endl;
    out << "    margin-right: auto;" << std::endl;
    out << "    margin-top: 20px;" << std::endl;
    out << "    margin-bottom: 20px;" << std::endl;
    out << "    padding-top: 5px;" << std::endl;
    out << "    padding-bottom: 5px;" << std::endl;
    out << "    padding-left: 0px;" << std::endl;
    out << "    padding-right: 0px;" << std::endl;
    out << "    text-align: center;" << std::endl;
    out << "    width: 200px;" << std::endl;
    out << "    background-color: #fff;" << std::endl;
    out << "    border-left: 0px;" << std::endl;
    out << "    border-right: 0px;" << std::endl;
    out << "    border-top: 0px;" << std::endl;
    out << "    border-bottom: 0px;" << std::endl;
    out << "}" << std::endl;
    // set the jump button style
    out << ".click-button" << std::endl;
    out << "{" << std::endl;
    out << "    border-width: 1px;" << std::endl;
    out << "    border-style: solid;" << std::endl;
    out << "    border-color: #eee;" << std::endl;
    out << "    background-color: #e5e9f8;" << std::endl;
    out << "    color:#038;" << std::endl;
    out << "    border-radius: 15px;" << std::endl;
    out << "    text-align: center;" << std::endl;
    out << "    text-decoration: none;" << std::endl;
    out << "    font-size: 16px;" << std::endl;
    out << "    cursor: pointer;" << std::endl;
    out << "    width: 200px;" << std::endl;
    out << "    height: 50px;" << std::endl;
    out << "}" << std::endl;
    // set the jump button style when mouse move on it
    out << ".click-button:hover" << std::endl;
    out << "{" << std::endl;
    out << "    background-color: #039;" << std::endl;
    out << "    color:white;" << std::endl;
    out << "}" << std::endl;
}
// generate prettyprint css for detail.html
void HtmlPrinter::generate_prettyprint_css()
{
    //create prettyprint.css file
    std::string out_filename = output_path + "/css/prettyprint.css";
    std::ofstream out(out_filename, std::ios::out | std::ios::trunc);
    if (!out.is_open())
    {
        INFO("Error when creating the prettyprint.css file\n");
        return;
    }
    //set color for different key words
    out << ".pln{" << std::endl;
    out << "  color: #000" << std::endl;
    out << "}" << std::endl;
    out << "@media screen {" << std::endl;
    out << "  .str {" << std::endl;
    out << "      color: #080" << std::endl;
    out << "   }" << std::endl;
    out << "  .kwd {" << std::endl;
    out << "      color: #008" << std::endl;
    out << "  }" << std::endl;
    out << "  .com {" << std::endl;
    out << "      color: #800" << std::endl;
    out << "  }" << std::endl;
    out << "  .typ {" << std::endl;
    out << "      color: #606" << std::endl;
    out << "  }" << std::endl;
    out << "  .lit {" << std::endl;
    out << "      color: #066" << std::endl;
    out << "  }" << std::endl;
    out << "  .pun,.opn,.clo {" << std::endl;
    out << "      color: #660" << std::endl;
    out << "  }" << std::endl;
    out << "  .tag {" << std::endl;
    out << "      color: #008" << std::endl;
    out << "  }" << std::endl;
    out << "  .atn {" << std::endl;
    out << "      color: #606" << std::endl;
    out << "  }" << std::endl;
    out << "  .atv {" << std::endl;
    out << "      color: #080" << std::endl;
    out << "  }" << std::endl;
    out << "  .dec,.var {" << std::endl;
    out << "      color: #606" << std::endl;
    out << "  }" << std::endl;
    out << "  .fun {" << std::endl;
    out << "      color: red" << std::endl;
    out << "  }" << std::endl;
    out << "}" << std::endl;
    out << "@media print,projection {" << std::endl;
    out << "  .str {" << std::endl;
    out << "      color: #060" << std::endl;
    out << "  }" << std::endl;
    out << "  .kwd {" << std::endl;
    out << "      color: #006;" << std::endl;
    out << "      font-weight: bold" << std::endl;
    out << "  }" << std::endl;
    out << "  .com {" << std::endl;
    out << "      color: #600;" << std::endl;
    out << "      font-style: italic" << std::endl;
    out << "  }" << std::endl;
    out << "  .typ {" << std::endl;
    out << "      color: #404;" << std::endl;
    out << "      font-weight: bold" << std::endl;
    out << "  }" << std::endl;
    out << "  .lit {" << std::endl;
    out << "      color: #044" << std::endl;
    out << "  }" << std::endl;
    out << "  .pun,.opn,.clo {" << std::endl;
    out << "      color: #440" << std::endl;
    out << "  }" << std::endl;
    out << "  .tag {" << std::endl;
    out << "      color: #006;" << std::endl;
    out << "      font-weight: bold" << std::endl;
    out << "  }" << std::endl;
    out << "  .atn {" << std::endl;
    out << "      color: #404" << std::endl;
    out << "  }" << std::endl;
    out << "  .atv {" << std::endl;
    out << "      color: #060" << std::endl;
    out << "  }" << std::endl;
    out << "}" << std::endl;
    //The detailed information outputs the source code in the form 
    //of a list, and each line of html code corresponds to the 
    //corresponding line of code in the source code file.
    //set the list wrapper style
    out << "pre.prettyprint {" << std::endl;
    out << "  margin-top: 0;" << std::endl;
    out << "  margin-bottom: 0;" << std::endl;
    out << "  margin-left: 0;" << std::endl;
    out << "  margin-right: 0;" << std::endl;
    out << "  padding-top: 0px;" << std::endl;
    out << "  padding-bottom: 0px;" << std::endl;
    out << "  padding-left: 0px;" << std::endl;
    out << "  padding-right: 0px;" << std::endl;
    out << "  border-top-width: 5px;" << std::endl;
    out << "  border-bottom-width: 10px;" << std::endl;
    out << "  border-left-width: 5px;" << std::endl;
    out << "  border-right-width: 10px;" << std::endl;
    out << "  border-color: #eee;" << std::endl;
    out << "  border-style: outset;" << std::endl;
    out << "  border-top-left-radius: 10px;" << std::endl;
    out << "  border-top-right-radius: 5px;" << std::endl;
    out << "  border-bottom-left-radius: 5px;" << std::endl;
    out << "  border-bottom-right-radius: 2px;" << std::endl;
    out << "}" << std::endl;
    //set the line number entry style
    out << "ol.linenums {" << std::endl;
    out << "  margin-top: 0;" << std::endl;
    out << "  margin-bottom: 0;" << std::endl;
    out << "  margin-left: 0;" << std::endl;
    out << "  margin-right: 0;" << std::endl;
    out << "  padding-top: 0px;" << std::endl;
    out << "  padding-bottom: 0px;" << std::endl;
    out << "  padding-left: 50px;" << std::endl;
    out << "  padding-right: 0px;" << std::endl;
    out << "  color:#008;" << std::endl;
    out << "  background: #ddd;" << std::endl;
    out << "  border-top-left-radius: 5px;" << std::endl;
    out << "  border-top-right-radius: 2px;" << std::endl;
    out << "  border-bottom-left-radius: 2px;" << std::endl;
    out << "  border-bottom-right-radius: 0px;" << std::endl;
    out << "}" << std::endl;
    //set source code list node style
    out << "li{" << std::endl;
    out << "  background: #fff;" << std::endl;
    out << "  margin-top: 0;" << std::endl;
    out << "  margin-bottom: 0;" << std::endl;
    out << "  margin-left: 0;" << std::endl;
    out << "  margin-right: 0;" << std::endl;
    out << "  border-top-width: 0px;" << std::endl;
    out << "  border-bottom-width: 0px;" << std::endl;
    out << "  border-left-width: 5px;" << std::endl;
    out << "  border-right-width: 0px;" << std::endl;
    out << "  padding-top: 0px;" << std::endl;
    out << "  padding-bottom: 0px;" << std::endl;
    out << "  padding-left: 5px;" << std::endl;
    out << "  padding-right: 1px;" << std::endl;
    out << "  border-style: solid;" << std::endl;
    out << "  border-color: #ddd;" << std::endl;
    out << "  border-top-left-radius: 0px;" << std::endl;
    out << "  border-top-right-radius: 0px;" << std::endl;
    out << "  border-bottom-left-radius: 0px;" << std::endl;
    out << "  border-bottom-right-radius: 0px;" << std::endl;
    out << "}" << std::endl;
    //set white for even lines
    out << "li.L0,li.L2,li.L4,li.L6,li.L8 {" << std::endl;
    out << "  background: #fff" << std::endl;
    out << "}" << std::endl;
    //set gray for odd lines
    out << "li.L1,li.L3,li.L5,li.L7,li.L9 {" << std::endl;
    out << "  background: #eee" << std::endl;
    out << "}" << std::endl;
    out.close();
}
// generate prettyprint javascript for detail.html
void HtmlPrinter::generate_prettyprint_js()
{
    //create prettyprint.js file
    std::string out_filename = output_path + "/js/prettyprint.js";
    std::ofstream out(out_filename, std::ios::out | std::ios::trunc);
    if (!out.is_open())
    {
        INFO("Error when creating the prettyprint.js file\n");
        return;
    }
    //inspired from google pretty print js.
    out << "!" << std::endl;
    out << "function() {" << std::endl;
    out << "	var q = null;" << std::endl;
    out << "	window.PR_SHOULD_USE_CONTINUATION = !0;" << std::endl;
    out << "	(function() {" << std::endl;
    out << "		function S(a) {" << std::endl;
    out << "			function d(e) {" << std::endl;
    out << "				var b = e.charCodeAt(0);" << std::endl;
    out << "				if (b !== 92) return b;" << std::endl;
    out << "				var a = e.charAt(1);" << std::endl;
    out << "				return (b = r[a]) ? b : \"0\" <= a && "
           "a <= \"7\" ? parseInt(e.substring(1), 8) : a === \"u\" || a === "
           "\"x\" ? parseInt(e.substring(2), 16) : e.charCodeAt(1)"
        << std::endl;
    out << "			}" << std::endl;
    out << "			function g(e) {" << std::endl;
    out << "				if (e < 32) return (e < 16 ? "
           "\"\\\\x0\" : \"\\\\x\") + e.toString(16);"
        << std::endl;
    out << "				e = String.fromCharCode(e);" << std::endl;
    out << "				return e === \"\\\\\" || e === \"-\" "
           "|| e === \"]\" || e === \"^\" ? \"\\\\\" + e : e"
        << std::endl;
    out << "			}" << std::endl;
    out << "			function b(e) {" << std::endl;
    out << "				var b = e.substring(1, e.length - "
           "1).match(/"
           "\\\\u[\\dA-Fa-f]{4}|\\\\x[\\dA-Fa-f]{2}|\\\\[0-3][0-7]{0,2}|\\\\[0-7]"
           "{1,2}|\\\\[\\S\\s]|[^\\\\]/g),"
        << std::endl;
    out << "					e = []," << std::endl;
    out << "					a = b[0] === \"^\"," << std::endl;
    out << "					c = [\"[\"];" << std::endl;
    out << "				a && c.push(\"^\");" << std::endl;
    out << "				for (var a = a ? 1 : 0, f = b.length; "
           "a < f; ++a) {"
        << std::endl;
    out << "					var h = b[a];" << std::endl;
    out << "					if (/\\\\[bdsw]/i.test(h)) c.push(h);" << std::endl;
    out << "					else {" << std::endl;
    out << "						var h = d(h)," << std::endl;
    out << "							l;" << std::endl;
    out << "						a + 2 < f && \"-\" === "
           "b[a + 1] ? (l = d(b[a + 2]), a += 2) : l = h;"
        << std::endl;
    out << "						e.push([h, l]);" << std::endl;
    out << "						l < 65 || h > 122 || "
           "(l < 65 || h > 90 || e.push([Math.max(65, h) | 32, Math.min(l, 90) | "
           "32]), l < 97 || h > 122 || e.push([Math.max(97, h) & -33, "
           "Math.min(l, 122) & -33]))"
        << std::endl;
    out << "					}" << std::endl;
    out << "				}" << std::endl;
    out << "				e.sort(function(e, a) {" << std::endl;
    out << "					return e[0] - a[0] || a[1] - "
           "e[1]"
        << std::endl;
    out << "				});" << std::endl;
    out << "				b = [];" << std::endl;
    out << "				f = [];" << std::endl;
    out << "				for (a = 0; a < e.length; ++a) h = "
           "e[a], h[0] <= f[1] + 1 ? f[1] = Math.max(f[1], h[1]) : b.push(f = h);"
        << std::endl;
    out << "				for (a = 0; a < b.length; ++a) h = "
           "b[a], c.push(g(h[0])), h[1] > h[0] && (h[1] + 1 > h[0] && "
           "c.push(\"-\"), c.push(g(h[1])));"
        << std::endl;
    out << "				c.push(\"]\");" << std::endl;
    out << "				return c.join(\"\")" << std::endl;
    out << "			}" << std::endl;
    out << "			function s(e) {" << std::endl;
    out << "				for (var a = "
           "e.source.match(/"
           "\\[(?:[^\\\\\\]]|\\\\[\\S\\s])*]|\\\\u[\\dA-Fa-f]{4}|\\\\x[\\dA-Fa-f]"
           "{2}|\\\\\\d+|\\\\[^\\dux]|\\(\\?[!:=]|[()^]|[^()[\\\\^]+/g), c = "
           "a.length, d = [], f = 0, h = 0; f < c; ++f) {"
        << std::endl;
    out << "					var l = a[f];" << std::endl;
    out << "					l === \"(\" ? ++h : \"\\\\\" "
           "=== l.charAt(0) && (l = +l.substring(1)) && (l <= h ? d[l] = -1 : "
           "a[f] = g(l))"
        << std::endl;
    out << "				}" << std::endl;
    out << "				for (f = 1; f < d.length; ++f) - 1 === "
           "d[f] && (d[f] = ++x);"
        << std::endl;
    out << "				for (h = f = 0; f < c; ++f) l = a[f], "
           "l === \"(\" ? (++h, d[h] || (a[f] = \"(?:\")) : \"\\\\\" === "
           "l.charAt(0) && (l = +l.substring(1)) && l <= h && (a[f] = \"\\\\\" + "
           "d[l]);"
        << std::endl;
    out << "				for (f = 0; f < c; ++f)\"^\" === a[f] "
           "&& \"^\" !== a[f + 1] && (a[f] = \"\");"
        << std::endl;
    out << "				if (e.ignoreCase && m) for (f = 0; f < "
           "c; ++f) l = a[f], e = l.charAt(0), l.length >= 2 && e === \"[\" ? "
           "a[f] = b(l) : e !== \"\\\\\" && (a[f] = l.replace(/[A-Za-z]/g, "
           "function(a) {"
        << std::endl;
    out << "					a = a.charCodeAt(0);" << std::endl;
    out << "					return \"[\" + "
           "String.fromCharCode(a & -33, a | 32) + \"]\""
        << std::endl;
    out << "				}));" << std::endl;
    out << "				return a.join(\"\")" << std::endl;
    out << "			}" << std::endl;
    out << "			for (var x = 0, m = !1, j = !1, k = 0, c = "
           "a.length; k < c; ++k) {"
        << std::endl;
    out << "				var i = a[k];" << std::endl;
    out << "				if (i.ignoreCase) j = !0;" << std::endl;
    out << "				else if "
           "(/[a-z]/i.test(i.source.replace(/"
           "\\\\u[\\da-f]{4}|\\\\x[\\da-f]{2}|\\\\[^UXux]/gi, \"\"))) {"
        << std::endl;
    out << "					m = !0;" << std::endl;
    out << "					j = !1;" << std::endl;
    out << "					break" << std::endl;
    out << "				}" << std::endl;
    out << "			}" << std::endl;
    out << "			for (var r = {" << std::endl;
    out << "				b: 8," << std::endl;
    out << "				t: 9," << std::endl;
    out << "				n: 10," << std::endl;
    out << "				v: 11," << std::endl;
    out << "				f: 12," << std::endl;
    out << "				r: 13" << std::endl;
    out << "			}, n = [], k = 0, c = a.length; k < c; ++k) {" << std::endl;
    out << "				i = a[k];" << std::endl;
    out << "				if (i.global || i.multiline) throw "
           "Error(\"\" + i);"
        << std::endl;
    out << "				n.push(\"(?:\" + s(i) + \")\")" << std::endl;
    out << "			}" << std::endl;
    out << "			return RegExp(n.join(\"|\"), j ? \"gi\" : "
           "\"g\")"
        << std::endl;
    out << "		}" << std::endl;
    out << "		function T(a, d) {" << std::endl;
    out << "			function g(a) {" << std::endl;
    out << "				var c = a.nodeType;" << std::endl;
    out << "				if (c == 1) {" << std::endl;
    out << "					if (!b.test(a.className)) {" << std::endl;
    out << "						for (c = a.firstChild; "
           "c; c = c.nextSibling) g(c);"
        << std::endl;
    out << "						c = "
           "a.nodeName.toLowerCase();"
        << std::endl;
    out << "						if (\"br\" === c || "
           "\"li\" === c) s[j] = \"\\n\", m[j << 1] = x++, m[j++ << 1 | 1] = a"
        << std::endl;
    out << "					}" << std::endl;
    out << "				} else if (c == 3 || c == 4) c = "
           "a.nodeValue, c.length && (c = d ? c.replace(/\\r\\n?/g, \"\\n\") : "
           "c.replace(/[\\t\\n\\r ]+/g, \" \"), s[j] = c, m[j << 1] = x, x += "
           "c.length, m[j++ << 1 | 1] = a)"
        << std::endl;
    out << "			}" << std::endl;
    out << "			var b = /(?:^|\\s)nocode(?:\\s|$)/," << std::endl;
    out << "				s = []," << std::endl;
    out << "				x = 0," << std::endl;
    out << "				m = []," << std::endl;
    out << "				j = 0;" << std::endl;
    out << "			g(a);" << std::endl;
    out << "			return {" << std::endl;
    out << "				a: s.join(\"\").replace(/\\n$/, \"\")," << std::endl;
    out << "				d: m" << std::endl;
    out << "			}" << std::endl;
    out << "		}" << std::endl;
    out << "		function H(a, d, g, b) {" << std::endl;
    out << "			d && (a = {" << std::endl;
    out << "				a: d," << std::endl;
    out << "				e: a" << std::endl;
    out << "			}, g(a), b.push.apply(b, a.g))" << std::endl;
    out << "		}" << std::endl;
    out << "		function U(a) {" << std::endl;
    out << "			for (var d = void 0, g = a.firstChild; g; g = "
           "g.nextSibling) var b = g.nodeType,"
        << std::endl;
    out << "				d = b === 1 ? d ? a : g : b === 3 ? "
           "V.test(g.nodeValue) ? a : d : d;"
        << std::endl;
    out << "			return d === a ? void 0 : d" << std::endl;
    out << "		}" << std::endl;
    out << "		function C(a, d) {" << std::endl;
    out << "			function g(a) {" << std::endl;
    out << "				for (var j = a.e, k = [j, \"pln\"], c "
           "= 0, i = a.a.match(s) || [], r = {}, n = 0, e = i.length; n < e; "
           "++n) {"
        << std::endl;
    out << "					var z = i[n]," << std::endl;
    out << "						w = r[z]," << std::endl;
    out << "						t = void 0," << std::endl;
    out << "						f;" << std::endl;
    out << "					if (typeof w === \"string\") f "
           "= !1;"
        << std::endl;
    out << "					else {" << std::endl;
    out << "						var h = b[z.charAt(0)];" << std::endl;
    out << "						if (h) t = "
           "z.match(h[1]), w = h[0];"
        << std::endl;
    out << "						else {" << std::endl;
    out << "							for (f = 0; f "
           "< x; ++f) if (h = d[f], t = z.match(h[1])) {"
        << std::endl;
    out << "								w = h[0];" << std::endl;
    out << "								break" << std::endl;
    out << "							}" << std::endl;
    out << "							t || (w = \"pln\")" << std::endl;
    out << "						}" << std::endl;
    out << "						if ((f = w.length >= 5 "
           "&& \"lang-\" === w.substring(0, 5)) && !(t && typeof t[1] === "
           "\"string\")) f = !1, w = \"src\";"
        << std::endl;
    out << "						f || (r[z] = w)" << std::endl;
    out << "					}" << std::endl;
    out << "					h = c;" << std::endl;
    out << "					c += z.length;" << std::endl;
    out << "					if (f) {" << std::endl;
    out << "						f = t[1];" << std::endl;
    out << "						var l = z.indexOf(f)," << std::endl;
    out << "							B = l + f.length;" << std::endl;
    out << "						t[2] && (B = z.length "
           "- t[2].length, l = B - f.length);"
        << std::endl;
    out << "						w = w.substring(5);" << std::endl;
    out << "						H(j + h, "
           "z.substring(0, l), g, k);"
        << std::endl;
    out << "						H(j + h + l, f, I(w, "
           "f), k);"
        << std::endl;
    out << "						H(j + h + B, "
           "z.substring(B), g, k)"
        << std::endl;
    out << "					} else k.push(j + h, w)" << std::endl;
    out << "				}" << std::endl;
    out << "				a.g = k" << std::endl;
    out << "			}" << std::endl;
    out << "			var b = {}," << std::endl;
    out << "				s;" << std::endl;
    out << "			(function() {" << std::endl;
    out << "				for (var g = a.concat(d), j = [], k = "
           "{}, c = 0, i = g.length; c < i; ++c) {"
        << std::endl;
    out << "					var r = g[c]," << std::endl;
    out << "						n = r[3];" << std::endl;
    out << "					if (n) for (var e = n.length; "
           "--e >= 0;) b[n.charAt(e)] = r;"
        << std::endl;
    out << "					r = r[1];" << std::endl;
    out << "					n = \"\" + r;" << std::endl;
    out << "					k.hasOwnProperty(n) || "
           "(j.push(r), k[n] = q)"
        << std::endl;
    out << "				}" << std::endl;
    out << "				j.push(/[\\S\\s]/);" << std::endl;
    out << "				s = S(j)" << std::endl;
    out << "			})();" << std::endl;
    out << "			var x = d.length;" << std::endl;
    out << "			return g" << std::endl;
    out << "		}" << std::endl;
    out << "		function v(a) {" << std::endl;
    out << "			var d = []," << std::endl;
    out << "				g = [];" << std::endl;
    out << "			a.tripleQuotedStrings ? d.push([\"str\", "
           "/^(?:\'\'\'(?:[^\'\\\\]|\\\\[\\S\\s]|\'\'?(?=[^\']))*(?:\'\'\'|$)|"
           "\"\"\"(?:[^\"\\\\]|\\\\[\\S\\s]|\"\"?(?=[^\"]))*(?:\"\"\"|$)|\'(?:[^"
           "\'\\\\]|\\\\[\\S\\s])*(?:\'|$)|\"(?:[^\"\\\\]|\\\\[\\S\\s])*(?:\"|$))"
           "/, q, \"\'\\\"\"]) : a.multiLineStrings ? d.push([\"str\", "
           "/^(?:\'(?:[^\'\\\\]|\\\\[\\S\\s])*(?:\'|$)|\"(?:[^\"\\\\]|\\\\["
           "\\S\\s])*(?:\"|$)|`(?:[^\\\\`]|\\\\[\\S\\s])*(?:`|$))/, q, "
           "\"\'\\\"`\"]) : d.push([\"str\", "
           "/^(?:\'(?:[^\\n\\r'\\\\]|\\\\.)*(?:\'|$)|\"(?:[^\\n\\r\"\\\\]|\\\\.)*"
           "(?:\"|$))/, q, \"\\\"\'\"]);"
        << std::endl;
    out << "			a.verbatimStrings && g.push([\"str\", "
           "/^@\"(?:[^\"]|\"\")*(?:\"|$)/, q]);"
        << std::endl;
    out << "			var b = a.hashComments;" << std::endl;
    out << "			b && (a.cStyleComments ? (b > 1 ? "
           "d.push([\"com\", /^#(?:##(?:[^#]|#(?!##))*(?:###|$)|.*)/, q, \"#\"]) "
           ": d.push([\"com\", "
           "/^#(?:(?:define|e(?:l|nd)if|else|error|ifn?def|include|line|pragma|"
           "undef|warning)\\b|[^\\n\\r]*)/, q, \"#\"]), g.push([\"str\", "
           "/^<(?:(?:(?:\\.\\.\\/)*|\\/?)(?:[\\w-]+(?:\\/"
           "[\\w-]+)+)?[\\w-]+\\.h(?:h|pp|\\+\\+)?|[a-z]\\w*)>/, q])) : "
           "d.push([\"com\", /^#[^\\n\\r]*/, q, \"#\"]));"
        << std::endl;
    out << "			a.cStyleComments && (g.push([\"com\", "
           "/^\\/\\/[^\\n\\r]*/, q]), g.push([\"com\", "
           "/^\\/\\*[\\S\\s]*?(?:\\*\\/|$)/, q]));"
        << std::endl;
    out << "			if (b = a.regexLiterals) {" << std::endl;
    out << "				var s = (b = b > 1 ? \"\" : "
           "\"\\n\\r\") ? \".\" : \"[\\\\S\\\\s]\";"
        << std::endl;
    out << "				g.push([\"lang-regex\", "
           "RegExp(\"^(?:^^\\\\.?|[+-]|[!=]=?=?|\\\\#|%=?|&&?=?|\\\\(|\\\\*=?|[+"
           "\\\\-]=|->|\\\\/"
           "=?|::?|<<?=?|>>?>?=?|,|;|\\\\?|@|\\\\[|~|{|\\\\^\\\\^?=?|\\\\|\\\\|?="
           "?|break|case|continue|delete|do|else|finally|instanceof|return|throw|"
           "try|typeof)\\\\s*(\" + (\"/(?=[^/*\" + b + "
           "\"])(?:[^/\\\\x5B\\\\x5C\" + b + \"]|\\\\x5C\" + s + "
           "\"|\\\\x5B(?:[^\\\\x5C\\\\x5D\" + b + \"]|\\\\x5C\" + s + "
           "\")*(?:\\\\x5D|$))+/\") + \")\")])"
        << std::endl;
    out << "			}(b = a.types) && g.push([\"typ\", b]);" << std::endl;
    out << "			b = (\"\" + a.keywords).replace(/^ | $/g, "
           "\"\");"
        << std::endl;
    out << "			b.length && g.push([\"kwd\", RegExp(\"^(?:\" + "
           "b.replace(/[\\s,]+/g, \"|\") + \")\\\\b\"), q]);"
        << std::endl;
    out << "			d.push([\"pln\", /^\\s+/, q, \" "
           "\\r\\n\\t\\u00a0\"]);"
        << std::endl;
    out << "			b = \"^.[^\\\\s\\\\w.$@'\\\"`/\\\\\\\\]*\";" << std::endl;
    out << "			a.regexLiterals && (b += \"(?!s*/)\");" << std::endl;
    out << "			g.push([\"lit\", /^@[$_a-z][\\w$@]*/i, q], "
           "[\"typ\", /^(?:[@_]?[A-Z]+[a-z][\\w$@]*|\\w+_t\\b)/, q], [\"pln\", "
           "/^[$_a-z][\\w$@]*/i, q], [\"lit\", "
           "/^(?:0x[\\da-f]+|(?:\\d(?:_\\d+)*\\d*(?:\\.\\d*)?|\\.\\d\\+)(?:e[+-]?"
           "\\d+)?)[a-z]*/i, q, \"0123456789\"], [\"pln\", /^\\\\[\\S\\s]?/, q], "
           "[\"pun\", RegExp(b), q]);"
        << std::endl;
    out << "			return C(d, g)" << std::endl;
    out << "		}" << std::endl;
    out << "		function J(a, d, g) {" << std::endl;
    out << "			function b(a) {" << std::endl;
    out << "				var c = a.nodeType;" << std::endl;
    out << "				if (c == 1 && !x.test(a.className)) if "
           "(\"br\" === a.nodeName) s(a), a.parentNode && "
           "a.parentNode.removeChild(a);"
        << std::endl;
    out << "				else for (a = a.firstChild; a; a = "
           "a.nextSibling) b(a);"
        << std::endl;
    out << "				else if ((c == 3 || c == 4) && g) {" << std::endl;
    out << "					var d = a.nodeValue," << std::endl;
    out << "						i = d.match(m);" << std::endl;
    out << "					if (i) c = d.substring(0, "
           "i.index), a.nodeValue = c, (d = d.substring(i.index + i[0].length)) "
           "&& a.parentNode.insertBefore(j.createTextNode(d), a.nextSibling), "
           "s(a), c || a.parentNode.removeChild(a)"
        << std::endl;
    out << "				}" << std::endl;
    out << "			}" << std::endl;
    out << "			function s(a) {" << std::endl;
    out << "				function b(a, c) {" << std::endl;
    out << "					var d = c ? a.cloneNode(!1) : a," << std::endl;
    out << "						e = a.parentNode;" << std::endl;
    out << "					if (e) {" << std::endl;
    out << "						var e = b(e, 1)," << std::endl;
    out << "							g = a.nextSibling;" << std::endl;
    out << "						e.appendChild(d);" << std::endl;
    out << "						for (var i = g; i; i = "
           "g) g = i.nextSibling, e.appendChild(i)"
        << std::endl;
    out << "					}" << std::endl;
    out << "					return d" << std::endl;
    out << "				}" << std::endl;
    out << "				for (; !a.nextSibling;) if (a = "
           "a.parentNode, !a) return;"
        << std::endl;
    out << "				for (var a = b(a.nextSibling, 0), d;" << std::endl;
    out << "				(d = a.parentNode) && d.nodeType === "
           "1;) a = d;"
        << std::endl;
    out << "				c.push(a)" << std::endl;
    out << "			}" << std::endl;
    out << "			for (var x = /(?:^|\\s)nocode(?:\\s|$)/, m = "
           "/\\r\\n?|\\n/, j = a.ownerDocument, k = j.createElement(\"li\"); "
           "a.firstChild;) k.appendChild(a.firstChild);"
        << std::endl;
    out << "			for (var c = [k], i = 0; i < c.length; ++i) "
           "b(c[i]);"
        << std::endl;
    out << "			d === (d | 0) && c[0].setAttribute(\"value\", "
           "d);"
        << std::endl;
    out << "			var r = j.createElement(\"ol\");" << std::endl;
    out << "			r.className = \"linenums\";" << std::endl;
    out << "			for (var d = Math.max(0, d - 1 | 0) || 0, i = "
           "0, n = c.length; i < n; ++i) k = c[i], k.className = \"L\" + (i + d) "
           "% 10, k.firstChild || k.appendChild(j.createTextNode(\"\\u00a0\")), "
           "r.appendChild(k);"
        << std::endl;
    out << "			a.appendChild(r)" << std::endl;
    out << "		}" << std::endl;
    out << "		function p(a, d) {" << std::endl;
    out << "			for (var g = d.length; --g >= 0;) {" << std::endl;
    out << "				var b = d[g];" << std::endl;
    out << "				F.hasOwnProperty(b) ? D.console && "
           "console.warn(\"cannot override language handler %s\", b) : F[b] = a"
        << std::endl;
    out << "			}" << std::endl;
    out << "		}" << std::endl;
    out << "		function I(a, d) {" << std::endl;
    out << "			if (!a || !F.hasOwnProperty(a)) a = "
           "/^\\s*</.test(d) ? \"default-markup\" : \"default-code\";"
        << std::endl;
    out << "			return F[a]" << std::endl;
    out << "		}" << std::endl;
    out << "		function K(a) {" << std::endl;
    out << "			var d = a.h;" << std::endl;
    out << "			try {" << std::endl;
    out << "				var g = T(a.c, a.i)," << std::endl;
    out << "					b = g.a;" << std::endl;
    out << "				a.a = b;" << std::endl;
    out << "				a.d = g.d;" << std::endl;
    out << "				a.e = 0;" << std::endl;
    out << "				I(d, b)(a);" << std::endl;
    out << "				var s = "
           "/\\bMSIE\\s(\\d+)/.exec(navigator.userAgent),"
        << std::endl;
    out << "					s = s && +s[1] <= 8," << std::endl;
    out << "					d = /\\n/g," << std::endl;
    out << "					x = a.a," << std::endl;
    out << "					m = x.length," << std::endl;
    out << "					g = 0," << std::endl;
    out << "					j = a.d," << std::endl;
    out << "					k = j.length," << std::endl;
    out << "					b = 0," << std::endl;
    out << "					c = a.g," << std::endl;
    out << "					i = c.length," << std::endl;
    out << "					r = 0;" << std::endl;
    out << "				c[i] = m;" << std::endl;
    out << "				var n, e;" << std::endl;
    out << "				for (e = n = 0; e < i;) c[e] !== c[e + "
           "2] ? (c[n++] = c[e++], c[n++] = c[e++]) : e += 2;"
        << std::endl;
    out << "				i = n;" << std::endl;
    out << "				for (e = n = 0; e < i;) {" << std::endl;
    out << "					for (var p = c[e], w = c[e + "
           "1], t = e + 2; t + 2 <= i && c[t + 1] === w;) t += 2;"
        << std::endl;
    out << "					c[n++] = p;" << std::endl;
    out << "					c[n++] = w;" << std::endl;
    out << "					e = t" << std::endl;
    out << "				}" << std::endl;
    out << "				c.length = n;" << std::endl;
    out << "				var f = a.c," << std::endl;
    out << "					h;" << std::endl;
    out << "				if (f) h = f.style.display, "
           "f.style.display = \"none\";"
        << std::endl;
    out << "				try {" << std::endl;
    out << "					for (; b < k;) {" << std::endl;
    out << "						var l = j[b + 2] || m," << std::endl;
    out << "							B = c[r + 2] || m," << std::endl;
    out << "							t = Math.min(l, B)," << std::endl;
    out << "							A = j[b + 1]," << std::endl;
    out << "							G;" << std::endl;
    out << "						if (A.nodeType !== 1 "
           "&& (G = x.substring(g, t))) {"
        << std::endl;
    out << "							s && (G = "
           "G.replace(d, \"\\r\"));"
        << std::endl;
    out << "							A.nodeValue = G;" << std::endl;
    out << "							var L = A.ownerDocument," << std::endl;
    out << "								o = "
           "L.createElement(\"span\");"
        << std::endl;
    out << "							o.className = c[r + 1];" << std::endl;
    out << "							var v = A.parentNode;" << std::endl;
    out << "							v.replaceChild(o, A);" << std::endl;
    out << "							o.appendChild(A);" << std::endl;
    out << "							g < l && (j[b "
           "+ 1] = A = L.createTextNode(x.substring(t, l)), v.insertBefore(A, "
           "o.nextSibling))"
        << std::endl;
    out << "						}" << std::endl;
    out << "						g = t;" << std::endl;
    out << "						g >= l && (b += 2);" << std::endl;
    out << "						g >= B && (r += 2)" << std::endl;
    out << "					}" << std::endl;
    out << "				} finally {" << std::endl;
    out << "					if (f) f.style.display = h" << std::endl;
    out << "				}" << std::endl;
    out << "			} catch (u) {" << std::endl;
    out << "				D.console && console.log(u && u.stack "
           "|| u)"
        << std::endl;
    out << "			}" << std::endl;
    out << "		}" << std::endl;
    out << "		var D = window," << std::endl;
    out << "			y = "
           "[\"break,continue,do,else,for,if,return,while\"],"
        << std::endl;
    out << "			E = [" << std::endl;
    out << "				[y, "
           "\"auto,case,char,const,default,double,enum,extern,float,goto,inline,"
           "int,long,register,short,signed,sizeof,static,struct,switch,typedef,"
           "union,unsigned,void,volatile\"], "
           "\"catch,class,delete,false,import,new,operator,private,protected,"
           "public,this,throw,true,try,typeof\"],"
        << std::endl;
    out << "			M = [E, "
           "\"alignof,align_union,asm,axiom,bool,concept,concept_map,const_cast,"
           "constexpr,decltype,delegate,dynamic_cast,explicit,export,friend,"
           "generic,late_check,mutable,namespace,nullptr,property,reinterpret_"
           "cast,static_assert,static_cast,template,typeid,typename,using,"
           "virtual,where\"],"
        << std::endl;
    out << "			N = [E, "
           "\"abstract,assert,boolean,byte,extends,final,finally,implements,"
           "import,instanceof,interface,null,native,package,strictfp,super,"
           "synchronized,throws,transient\"],"
        << std::endl;
    out << "			O = [N, "
           "\"as,base,by,checked,decimal,delegate,descending,dynamic,event,fixed,"
           "foreach,from,group,implicit,in,internal,into,is,let,lock,object,out,"
           "override,orderby,params,partial,readonly,ref,sbyte,sealed,stackalloc,"
           "string,select,uint,ulong,unchecked,unsafe,ushort,var,virtual,where\"]"
           ","
        << std::endl;
    out << "			E = [E, "
           "\"debugger,eval,export,function,get,null,set,undefined,var,with,"
           "Infinity,NaN\"],"
        << std::endl;
    out << "			P = [y, "
           "\"and,as,assert,class,def,del,elif,except,exec,finally,from,global,"
           "import,in,is,lambda,nonlocal,not,or,pass,print,raise,try,with,yield,"
           "False,True,None\"],"
        << std::endl;
    out << "			Q = [y, "
           "\"alias,and,begin,case,class,def,defined,elsif,end,ensure,false,in,"
           "module,next,nil,not,or,redo,rescue,retry,self,super,then,true,undef,"
           "unless,until,when,yield,BEGIN,END\"],"
        << std::endl;
    out << "			W = [y, "
           "\"as,assert,const,copy,drop,enum,extern,fail,false,fn,impl,let,log,"
           "loop,match,mod,move,mut,priv,pub,pure,ref,self,static,struct,true,"
           "trait,type,unsafe,use\"],"
        << std::endl;
    out << "			y = [y, "
           "\"case,done,elif,esac,eval,fi,function,in,local,set,then,until\"],"
        << std::endl;
    out << "			R = "
           "/^(DIR|FILE|vector|(de|priority_)?queue|list|stack|(const_)?iterator|"
           "(multi)?(set|map)|bitset|u?(int|float)\\d*)\\b/,"
        << std::endl;
    out << "			V = /\\S/," << std::endl;
    out << "			X = v({" << std::endl;
    out << "				keywords: [M, O, E, "
           "\"caller,delete,die,do,dump,elsif,eval,exit,foreach,for,goto,if,"
           "import,last,local,my,next,no,our,print,package,redo,require,sub,"
           "undef,unless,until,use,wantarray,while,BEGIN,END\", P, Q, y],"
        << std::endl;
    out << "				hashComments: !0," << std::endl;
    out << "				cStyleComments: !0," << std::endl;
    out << "				multiLineStrings: !0," << std::endl;
    out << "				regexLiterals: !0" << std::endl;
    out << "			})," << std::endl;
    out << "			F = {};" << std::endl;
    out << "		p(X, [\"default-code\"]);" << std::endl;
    out << "		p(C([], [" << std::endl;
    out << "			[\"pln\", /^[^<?]+/]," << std::endl;
    out << "			[\"dec\", /^<!\\w[^>]*(?:>|$)/]," << std::endl;
    out << "			[\"com\", /^<\\!--[\\S\\s]*?(?:--\\>|$)/]," << std::endl;
    out << "			[\"lang-\", /^<\\?([\\S\\s]+?)(?:\\?>|$)/]," << std::endl;
    out << "			[\"lang-\", /^<%([\\S\\s]+?)(?:%>|$)/]," << std::endl;
    out << "			[\"pun\", /^(?:<[%?]|[%?]>)/]," << std::endl;
    out << "			[\"lang-\", "
           "/^<xmp\\b[^>]*>([\\S\\s]+?)<\\/xmp\\b[^>]*>/i],"
        << std::endl;
    out << "			[\"lang-js\", "
           "/^<script\\b[^>]*>([\\S\\s]*?)(<\\/script\\b[^>]*>)/i],"
        << std::endl;
    out << "			[\"lang-css\", "
           "/^<style\\b[^>]*>([\\S\\s]*?)(<\\/style\\b[^>]*>)/i],"
        << std::endl;
    out << "			[\"lang-in.tag\", /^(<\\/?[a-z][^<>]*>)/i]" << std::endl;
    out << "		]), [\"default-markup\", \"htm\", \"html\", \"mxml\", "
           "\"xhtml\", \"xml\", \"xsl\"]);"
        << std::endl;
    out << "		p(C([" << std::endl;
    out << "			[\"pln\", /^\\s+/, q, \" \\t\\r\\n\"]," << std::endl;
    out << "			[\"atv\", /^(?:\"[^\"]*\"?|\'[^\']*\'?)/, q, "
           "\"\\\"\'\"]"
        << std::endl;
    out << "		], [" << std::endl;
    out << "			[\"tag\", "
           "/^^<\\/?[a-z](?:[\\w-.:]*\\w)?|\\/?>$/i],"
        << std::endl;
    out << "			[\"atn\", "
           "/^(?!style[\\s=]|on)[a-z](?:[\\w:-]*\\w)?/i],"
        << std::endl;
    out << "			[\"lang-uq.val\", "
           "/^=\\s*([^\\s\"\'>]*(?:[^\\s\"\'/>]|\\/(?=\\s)))/],"
        << std::endl;
    out << "			[\"pun\", /^[/<->]+/]," << std::endl;
    out << "			[\"lang-js\", /^on\\w+\\s*=\\s*\"([^\"]+)\"/i]," << std::endl;
    out << "			[\"lang-js\", /^on\\w+\\s*=\\s*\'([^\']+)\'/i]," << std::endl;
    out << "			[\"lang-js\", "
           "/^on\\w+\\s*=\\s*([^\\s\"\'>]+)/i],"
        << std::endl;
    out << "			[\"lang-css\", /^style\\s*=\\s*\"([^\"]+)\"/i]," << std::endl;
    out << "			[\"lang-css\", /^style\\s*=\\s*\'([^\']+)\'/i]," << std::endl;
    out << "			[\"lang-css\", "
           "/^style\\s*=\\s*([^\\s\"\'>]+)/i]"
        << std::endl;
    out << "		]), [\"in.tag\"]);" << std::endl;
    out << "		p(C([], [" << std::endl;
    out << "			[\"atv\", /^[\\S\\s]+/]" << std::endl;
    out << "		]), [\"uq.val\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: M," << std::endl;
    out << "			hashComments: !0," << std::endl;
    out << "			cStyleComments: !0," << std::endl;
    out << "			types: R" << std::endl;
    out << "		}), [\"c\", \"cc\", \"cpp\", \"cxx\", \"cyc\", \"m\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: \"null,true,false\"" << std::endl;
    out << "		}), [\"json\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: O," << std::endl;
    out << "			hashComments: !0," << std::endl;
    out << "			cStyleComments: !0," << std::endl;
    out << "			verbatimStrings: !0," << std::endl;
    out << "			types: R" << std::endl;
    out << "		}), [\"cs\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: N," << std::endl;
    out << "			cStyleComments: !0" << std::endl;
    out << "		}), [\"java\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: y," << std::endl;
    out << "			hashComments: !0," << std::endl;
    out << "			multiLineStrings: !0" << std::endl;
    out << "		}), [\"bash\", \"bsh\", \"csh\", \"sh\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: P," << std::endl;
    out << "			hashComments: !0," << std::endl;
    out << "			multiLineStrings: !0," << std::endl;
    out << "			tripleQuotedStrings: !0" << std::endl;
    out << "		}), [\"cv\", \"py\", \"python\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: "
           "\"caller,delete,die,do,dump,elsif,eval,exit,foreach,for,goto,if,"
           "import,last,local,my,next,no,our,print,package,redo,require,sub,"
           "undef,unless,until,use,wantarray,while,BEGIN,END\","
        << std::endl;
    out << "			hashComments: !0," << std::endl;
    out << "			multiLineStrings: !0," << std::endl;
    out << "			regexLiterals: 2" << std::endl;
    out << "		}), [\"perl\", \"pl\", \"pm\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: Q," << std::endl;
    out << "			hashComments: !0," << std::endl;
    out << "			multiLineStrings: !0," << std::endl;
    out << "			regexLiterals: !0" << std::endl;
    out << "		}), [\"rb\", \"ruby\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: E," << std::endl;
    out << "			cStyleComments: !0," << std::endl;
    out << "			regexLiterals: !0" << std::endl;
    out << "		}), [\"javascript\", \"js\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: "
           "\"all,and,by,catch,class,else,extends,false,finally,for,if,in,is,"
           "isnt,loop,new,no,not,null,of,off,on,or,return,super,then,throw,true,"
           "try,unless,until,when,while,yes\","
        << std::endl;
    out << "			hashComments: 3," << std::endl;
    out << "			cStyleComments: !0," << std::endl;
    out << "			multilineStrings: !0," << std::endl;
    out << "			tripleQuotedStrings: !0," << std::endl;
    out << "			regexLiterals: !0" << std::endl;
    out << "		}), [\"coffee\"]);" << std::endl;
    out << "		p(v({" << std::endl;
    out << "			keywords: W," << std::endl;
    out << "			cStyleComments: !0," << std::endl;
    out << "			multilineStrings: !0" << std::endl;
    out << "		}), [\"rc\", \"rs\", \"rust\"]);" << std::endl;
    out << "		p(C([], [" << std::endl;
    out << "			[\"str\", /^[\\S\\s]+/]" << std::endl;
    out << "		]), [\"regex\"]);" << std::endl;
    out << "		var Y = D.PR = {" << std::endl;
    out << "			createSimpleLexer: C," << std::endl;
    out << "			registerLangHandler: p," << std::endl;
    out << "			sourceDecorator: v," << std::endl;
    out << "			PR_ATTRIB_NAME: \"atn\"," << std::endl;
    out << "			PR_ATTRIB_VALUE: \"atv\"," << std::endl;
    out << "			PR_COMMENT: \"com\"," << std::endl;
    out << "			PR_DECLARATION: \"dec\"," << std::endl;
    out << "			PR_KEYWORD: \"kwd\"," << std::endl;
    out << "			PR_LITERAL: \"lit\"," << std::endl;
    out << "			PR_NOCODE: \"nocode\"," << std::endl;
    out << "			PR_PLAIN: \"pln\"," << std::endl;
    out << "			PR_PUNCTUATION: \"pun\"," << std::endl;
    out << "			PR_SOURCE: \"src\"," << std::endl;
    out << "			PR_STRING: \"str\"," << std::endl;
    out << "			PR_TAG: \"tag\"," << std::endl;
    out << "			PR_TYPE: \"typ\"," << std::endl;
    out << "			prettyPrintOne: D.prettyPrintOne = function(a, "
           "d, g) {"
        << std::endl;
    out << "				var b = document.createElement(\"div\");" << std::endl;
    out << "				b.innerHTML = \"<pre>\" + a + \"</pre>\";" << std::endl;
    out << "				b = b.firstChild;" << std::endl;
    out << "				g && J(b, g, !0);" << std::endl;
    out << "				K({" << std::endl;
    out << "					h: d," << std::endl;
    out << "					j: g," << std::endl;
    out << "					c: b," << std::endl;
    out << "					i: 1" << std::endl;
    out << "				});" << std::endl;
    out << "				return b.innerHTML" << std::endl;
    out << "			}," << std::endl;
    out << "			prettyPrint: D.prettyPrint = function(a, d) {" << std::endl;
    out << "				function g() {" << std::endl;
    out << "					for (var b = "
           "D.PR_SHOULD_USE_CONTINUATION ? c.now() + 250 : Infinity; i < "
           "p.length && c.now() < b; i++) {"
        << std::endl;
    out << "						for (var d = p[i], j = "
           "h, k = d; k = k.previousSibling;) {"
        << std::endl;
    out << "							var m = k.nodeType," << std::endl;
    out << "								o = (m "
           "=== 7 || m === 8) && k.nodeValue;"
        << std::endl;
    out << "							if (o ? "
           "!/^\\??prettify\\b/.test(o) : m !== 3 || /\\S/.test(k.nodeValue)) "
           "break;"
        << std::endl;
    out << "							if (o) {" << std::endl;
    out << "								j = {};" << std::endl;
    out << "								"
           "o.replace(/\\b(\\w+)=([\\w%+\\-.:]+)/g, function(a, b, c) {"
        << std::endl;
    out << "									j[b] = c" << std::endl;
    out << "								});" << std::endl;
    out << "								break" << std::endl;
    out << "							}" << std::endl;
    out << "						}" << std::endl;
    out << "						k = d.className;" << std::endl;
    out << "						if ((j !== h || "
           "e.test(k)) && !v.test(k)) {"
        << std::endl;
    out << "							m = !1;" << std::endl;
    out << "							for (o = "
           "d.parentNode; o; o = o.parentNode) if (f.test(o.tagName) && "
           "o.className && e.test(o.className)) {"
        << std::endl;
    out << "								m = !0;" << std::endl;
    out << "								break" << std::endl;
    out << "							}" << std::endl;
    out << "							if (!m) {" << std::endl;
    out << "								"
           "d.className += \" prettyprinted\";"
        << std::endl;
    out << "								m = j.lang;" << std::endl;
    out << "								if (!m) {" << std::endl;
    out << "								"
           "	var m = k.match(n),"
        << std::endl;
    out << "										y;" << std::endl;
    out << "								"
           "	if (!m && (y = U(d)) && t.test(y.tagName)) m = "
           "y.className.match(n);"
        << std::endl;
    out << "									m && (m = m[1])" << std::endl;
    out << "								}" << std::endl;
    out << "								if "
           "(w.test(d.tagName)) o = 1;"
        << std::endl;
    out << "								else "
           "var o = d.currentStyle,"
        << std::endl;
    out << "								"
           "	u = s.defaultView,"
        << std::endl;
    out << "								"
           "	o = (o = o ? o.whiteSpace : u && u.getComputedStyle ? "
           "u.getComputedStyle(d, q).getPropertyValue(\"white-space\") : 0) && "
           "\"pre\" === o.substring(0, 3);"
        << std::endl;
    out << "								u = j.linenums;" << std::endl;
    out << "								if "
           "(!(u = u === \"true\" || +u)) u = (u = "
           "k.match(/\\blinenums\\b(?::(\\d+))?/)) ? u[1] && u[1].length ? +u[1] "
           ": !0 : !1;"
        << std::endl;
    out << "								u && J(d, u, o);" << std::endl;
    out << "								r = {" << std::endl;
    out << "									h: m," << std::endl;
    out << "									c: d," << std::endl;
    out << "									j: u," << std::endl;
    out << "									i: o" << std::endl;
    out << "								};" << std::endl;
    out << "								K(r)" << std::endl;
    out << "							}" << std::endl;
    out << "						}" << std::endl;
    out << "					}" << std::endl;
    out << "					i < p.length ? setTimeout(g, "
           "250) : \"function\" === typeof a && a()"
        << std::endl;
    out << "				}" << std::endl;
    out << "				for (var b = d || document.body, s = "
           "b.ownerDocument || document, b = [b.getElementsByTagName(\"pre\"), "
           "b.getElementsByTagName(\"code\"), b.getElementsByTagName(\"xmp\")], "
           "p = [], m = 0; m < b.length; ++m) for (var j = 0, k = b[m].length; j "
           "< k; ++j) p.push(b[m][j]);"
        << std::endl;
    out << "				var b = q," << std::endl;
    out << "					c = Date;" << std::endl;
    out << "				c.now || (c = {" << std::endl;
    out << "					now: function() {" << std::endl;
    out << "						return +new Date" << std::endl;
    out << "					}" << std::endl;
    out << "				});" << std::endl;
    out << "				var i = 0," << std::endl;
    out << "					r, n = "
           "/\\blang(?:uage)?-([\\w.]+)(?!\\S)/,"
        << std::endl;
    out << "					e = /\\bprettyprint\\b/," << std::endl;
    out << "					v = /\\bprettyprinted\\b/," << std::endl;
    out << "					w = /pre|xmp/i," << std::endl;
    out << "					t = /^code$/i," << std::endl;
    out << "					f = /^(?:pre|code|xmp)$/i," << std::endl;
    out << "					h = {};" << std::endl;
    out << "				g()" << std::endl;
    out << "			}" << std::endl;
    out << "		};" << std::endl;
    out << "		typeof define === \"function\" && define.amd && "
           "define(\"google-code-prettify\", [], function() {"
        << std::endl;
    out << "			return Y" << std::endl;
    out << "		})" << std::endl;
    out << "	})();" << std::endl;
    out << "}()" << std::endl;
    out.close();
}
// generate index.html,show the info of all the diagnostics
void HtmlPrinter::generate_index_html()
{
    std::string out_filename = output_path + "/index.html";
    //create index html file
    std::ofstream out(out_filename, std::ios::out | std::ios::trunc);
    if (!out.is_open())
    {
        INFO("Error when creating the index.html file\n");
        return;
    }
    //output header information
    out << "<!DOCTYPE html>" << std::endl;
    out << "<html xmlns=\"http://www.w3.org/1999/xhtml\">" << std::endl;
    out << "    <head>" << std::endl;
    out << "        <meta charset=\"utf-8\"/>" << std::endl;
    out << "        <title>" << std::endl;
    out << "            hisa analyis result" << std::endl;
    out << "        </title>" << std::endl;
    out << "        <link rel=\"stylesheet\" type=\"text/css\" "
           "href=\"css/style.css\"/>"
        << std::endl;
    out << "    </head>" << std::endl;
    out << "    <body>" << std::endl;
    out << "        <h1 align=\"center\">" << std::endl;
    out << "            hisa analyze result" << std::endl;
    out << "        </h1>" << std::endl;
    //if statis output option is open, create a jump button for state html
    if (statis)
    {
        out << "        <h2 align=\"center\">" << std::endl;
        out << "            To See More Information About Files And Bugs Statistics" << std::endl;
        out << "        </h2>" << std::endl;
        out << "        <div id=\"button-wrapper\">" << std::endl;
        out << "            <a href=\"./stats.html\">" << std::endl;
        out << "                <button class=\"click-button\">" << std::endl;
        out << "                    Clicked Here" << std::endl;
        out << "                </button>" << std::endl;
        out << "            </a>" << std::endl;
        out << "        </div>" << std::endl;
    }
    //there are two entries for file name and detailed information by default,
    //so the column number is initialized to 2
    int col = 2;
    out << "        <h2 align=\"center\">" << std::endl;
    out << "            Bug Descriptions" << std::endl;
    out << "        </h2>" << std::endl;
    out << "        <table id=\"rounded-corner\">" << std::endl;
    out << "            <thead>" << std::endl;
    out << "                <tr>" << std::endl;
    out << "                    <th scope=\"col\" class=\"rounded-left\">" << std::endl;
    out << "                        File Name" << std::endl;
    out << "                    </th>" << std::endl;
    //if bug_type output options is open, add the entry of the bug type
    if (bug_type)
    {
        out << "                <th scope=\"col\">" << std::endl;
        out << "                    Bug Type" << std::endl;
        out << "                </th>" << std::endl;
        col++;
    }
    // if loc output options is open, add the entry of the location 
    if (loc)
    {
        out << "                <th scope=\"col\">" << std::endl;
        out << "                    Location" << std::endl;
        out << "                </th>" << std::endl;
        col++;
    }
    // if checker output options is open, add the entry of the checker name
    if (checker)
    {
        out << "                <th scope=\"col\">" << std::endl;
        out << "                    Checker" << std::endl;
        out << "                </th>" << std::endl;
        col++;
    }
    // if note output options is open, add the entry of the description information
    if (note)
    {
        out << "                <th scope=\"col\">" << std::endl;
        out << "                    Description" << std::endl;
        out << "                </th>" << std::endl;
        col++;
    }
    // if analy_time output options is open, add the entry of the analysis time
    if (analy_time)
    {
        out << "                <th scope=\"col\">" << std::endl;
        out << "                    Analysis time" << std::endl;
        out << "                </th>" << std::endl;
        col++;
    }
    out << "                    <th scope=\"col\" class=\"rounded-right\">" << std::endl;
    out << "                        Details" << std::endl;
    out << "                    </th>" << std::endl;
    out << "                </tr>" << std::endl;
    out << "            </thead>" << std::endl;
    //output detailed bug information
    out << "            <tbody>" << std::endl;
    auto file_stats = StatsHelper::instance()->get_file_stats_list();
    //get file statistics information from StatsHelper, for each item in file stats,
    //there is a bug type stats list in it. and for each item in type stats, there
    //is a bug decs stats list in it. then for each item in bug desc stats, output 
    //detailed information for it.
    for (auto &i : file_stats)
    {
        auto bug_type_stats = i.get_bug_stats_list();
        for (auto &j : bug_type_stats)
        {
            auto bug_decs_stats = j.get_bug_desc_list();
            for (auto &k : bug_decs_stats)
            {
                out << "                <tr>" << std::endl;
                if (i._filename != "")
                    out << "                    <td>" << i._filename << "</td>" << std::endl;
                else
                {
                    //The clang symbol execution logic will analyze the header files included in
                    //each file.If an error is analyzed, it will also report an error, but our 
                    //program shields the output of this error message, because we do not detect 
                    //the header file by default, so only the output An "unknown header file" 
                    // information and will not provide detailed information.
                    std::string temp = "unknown header file";
                    out << "                    <td>" << temp << "</td>" << std::endl;
                }
                if (bug_type)
                {
                    out << "                    <td>" << j._bug_type << "</td>" << std::endl;
                }
                if (loc)
                {
                    out << "                    <td>(" << k._line_number << ":" << k._column_number << ")</td>"
                        << std::endl;
                }
                if (checker)
                {
                    out << "                    <td>" << k._checker_name << "</td>" << std::endl;
                }
                if (note)
                {
                    out << "                    <td>" << k._description << "</td>" << std::endl;
                }
                if (analy_time)
                {
                    //When the file name is "unknown header file", because of we will not output error 
                    //information across files by default, the analysis time for this entry is zero
                    //Set the precision to two decimal places
                    out << "                    <td>" << std::setiosflags(std::ios::fixed) << std::setprecision(2)
                        << i.get_analysis_time() << "</td>" << std::endl;
                }
                if (i._filename != "")
                {
                    // get the output filename from the absolute path filename of i
                    std::string new_filename = i._filename;
                    // replace \ or / or . or : to - ,in order to generate a new filename
                    std::replace(new_filename.begin(), new_filename.end(), '/', '-');
                    std::replace(new_filename.begin(), new_filename.end(), '\\', '-');
                    std::replace(new_filename.begin(), new_filename.end(), '.', '-');
                    std::replace(new_filename.begin(), new_filename.end(), ':', '-');
                    std::string out_filename ="./modules/" + new_filename + ".html";
                    //create the link to detail html
                    out << "                    <td><a href=\"" << out_filename << "\">" << i._filename << "</td>"
                        << std::endl;
                }
                else
                {
                    // When the file name is "unknown header file", because of we will not output error
                    // information across files by default, the detailed infomation will not be provided
                    out << "                    <td>" << std::endl;
                    out << "                        unknown header file, don't generate details for it" << std ::endl;
                    out << "                    </td>" << std::endl;
                }
                out << "                </tr>" << std::endl;
            }
        }
    }
    out << "            </tbody>" << std::endl;
    out << "            <tfoot>" << std::endl;
    out << "                <tr>" << std::endl;
    out << "                <td class=\"rounded-foot-left\"></td>" << std::endl;
    for (int i = 2; i < col; i++)
        out << "                <td></td>" << std::endl;
    out << "                <td class=\"rounded-foot-right\"></td>" << std::endl;
    out << "                </tr>" << std::endl;
    out << "            </tfoot>" << std::endl;
    out << "        </table>" << std::endl;
    out << "    </body>" << std::endl;
    out << "</html>" << std::endl;
    out.close();
}
// if user open the stats output options, generate stats.index to show the statistic infomation
void HtmlPrinter::generate_stats_html()
{
    std::string out_filename = output_path + "/stats.html";
    std::ofstream out(out_filename, std::ios::out | std::ios::trunc);
    if (!out.is_open())
    {
        INFO("Error when creating the stats.html file\n");
        return;
    }
    //output header information
    out << "<!DOCTYPE html>" << std::endl;
    out << "<html xmlns=\"http://www.w3.org/1999/xhtml\">" << std::endl;
    out << "    <head>" << std::endl;
    out << "        <meta charset=\"utf-8\"/>" << std::endl;
    out << "        <title>" << std::endl;
    out << "            hisa analyis statistic" << std::endl;
    out << "        </title>" << std::endl;
    out << "        <link rel=\"stylesheet\" type=\"text/css\" "
           "href=\"css/stats.css\"/>"
        << std::endl;
    out << "    </head>" << std::endl;
    out << "    <body>" << std::endl;
    out << "        <h1 align=\"center\">" << std::endl;
    out << "            hisa analyze result" << std::endl;
    out << "        </h1>" << std::endl;
    //output thanks info
    out << "        <div id=\"thanks-info\">" << std::endl;
    out << "            <p>" << std::endl;
    out << "                Thanks for using our C/C++ code static analysis tool hisa" << std::endl;
    out << "            </p>" << std::endl;
    out << "        </div>" << std::endl;
    auto file_stats = StatsHelper::instance()->get_file_stats_list();
    //output file statistics overall result information
    out << "        <div id=\"result-info\"" << std::endl;
    out << "            <p>" << std::endl;
    out << "                Here are the analysis results about files." << std::endl;
    out << "            </p>" << std::endl;
    out << "            <p>" << std::endl;
    int file_num = StatsHelper::instance()->get_all_file_num();
    out << "                In this static analysis, a total of " << file_num << " documents were detected."
        << std::endl;
    out << "            </p>" << std::endl;
    out << "            <p>" << std::endl;
    double all_analyze_time = StatsHelper::instance()->get_all_analysis_time();
    out << "                The time to analyze all files is " << all_analyze_time << "s." << std::endl;
    out << "            </p>" << std::endl;
    int error_num = 0;
    for (auto &i : file_stats)
    {
        if (i.get_all_bug_num() != 0)
            error_num++;
    }
    int correct_num = file_num - error_num;
    out << "            <p>" << std::endl;
    out << "                The number of files detected to be correct is " << correct_num << "." << std::endl;
    out << "            </p>" << std::endl;
    out << "            <p>" << std::endl;
    out << "                The number of files detected to be in error is " << error_num << "." << std::endl;
    out << "            </p>" << std::endl;
    out << "            <p>" << std::endl;
    out << "                Following is the file statistics information." << std::endl;
    out << "            </p>" << std::endl;
    out << "        </div>" << std::endl;
    //output the detailed file statistics information
    out << "        <h2 align=\"center\">" << std::endl;
    out << "            File Statistics" << std::endl;
    out << "        </h2>" << std::endl;
    out << "        <table id=\"rounded-corner\">" << std::endl;
    out << "            <thead>" << std::endl;
    out << "                <tr>" << std::endl;
    out << "                    <th scope=\"col\" class=\"rounded-left\">" << std::endl;
    out << "                        File Name" << std::endl;
    out << "                    </th>" << std::endl;
    out << "                    <th scope=\"col\">" << std::endl;
    out << "                        Analysis Time" << std::endl;
    out << "                    </th>" << std::endl;
    out << "                    <th scope=\"col\" class=\"rounded-right\">" << std::endl;
    out << "                        Correct or Error" << std::endl;
    out << "                     </th>" << std::endl;
    out << "                </tr>" << std::endl;
    out << "            </thead>" << std::endl;
    out << "            <tbody>" << std::endl;
    for (auto &i : file_stats)
    {
        out << "                <tr>" << std::endl;
        out << "                    <td>" << std::endl;
        out << "                        "<<i._filename << std::endl;
        out << "                    </td>" << std::endl;
        out << "                    <td>" << std::endl;
        out << "                        " << std::setiosflags(std::ios::fixed) << std::setprecision(2)
            << i.get_analysis_time() << std::endl;
        out << "                    </td>" << std::endl;
        bool correct = false;
        if (i.get_all_bug_num() == 0)
        {
            correct = true;
        }
        out << "                    <td>" << std::endl;
        if (correct)
        {
            //output the correct words in green
            out << "                        <p>" << std::endl;
            out << "                            <font color=#00ff00>" << std::endl;
            out << "                                Correct" << std::endl;
            out << "                            </font>" << std::endl;
            out << "                        </p>" << std::endl;
        }
        else
        {
            // output the error words in red
            out << "                        <p>" << std::endl;
            out << "                            <font color=#ff0000>" << std::endl;
            out << "                                Error" << std::endl;
            out << "                            </font>" << std::endl;
            out << "                        </p>" << std::endl;
        }
        out << "                    </td>" << std::endl;
        out << "                </tr>" << std::endl;
    }
    out << "            </tbody>" << std::endl;
    out << "            <tfoot>" << std::endl;
    out << "                <tr>" << std::endl;
    out << "                <td class=\"rounded-foot-left\"></td>" << std::endl;
    out << "                <td></td>" << std::endl;
    out << "                <td class=\"rounded-foot-right\"></td>" << std::endl;
    out << "                </tr>" << std::endl;
    out << "            </tfoot>" << std::endl;
    out << "        </table>" << std::endl;
    auto bug_stats = StatsHelper::instance()->get_all_bug_stats_list();
    // output bug type statistics overall result information
    out << "        <div id=\"result-info\"" << std::endl;
    out << "            <p>" << std::endl;
    out << "                Here are the analysis results about bugss." << std::endl;
    out << "            </p>" << std::endl;
    int bug_type_num = StatsHelper::instance()->get_all_bug_type_num();
    out << "            <p>" << std::endl;
    out << "                In this static analysis, the number of bug types detected is " << bug_type_num << "."
        << std::endl;
    out << "            </p>" << std::endl;
    int bug_num = StatsHelper::instance()->get_all_bug_num();
    out << "            <p>" << std::endl;
    out << "                The number of all bugs is "<<bug_num<<"." << std::endl;
    out << "            </p>" << std::endl;
    out << "            <p>" << std::endl;
    out << "                Following is the bug statistic information." << std::endl;
    out << "            </p>" << std::endl;
    out << "        </div>" << std::endl;
    // output the detailed bug type statistics information
    out << "        <h2 align=\"center\">" << std::endl;
    out << "            Bug Statistics" << std::endl;
    out << "        </h2>" << std::endl;
    out << "        <table id=\"rounded-corner\">" << std::endl;
    out << "            <thead>" << std::endl;
    out << "                <tr>" << std::endl;
    out << "                    <th scope=\"col\" class=\"rounded-left\">" << std::endl;
    out << "                        Bug Type" << std::endl;
    out << "                    </th>" << std::endl;
    out << "                    <th scope=\"col\">" << std::endl;
    out << "                        Number" << std::endl;
    out << "                    </th>" << std::endl;
    out << "                    <th scope=\"col\" class=\"rounded-right\">" << std::endl;
    out << "                        Persent" << std::endl;
    out << "                     </th>" << std::endl;
    out << "                </tr>" << std::endl;
    out << "            </thead>" << std::endl;
    out << "            <tbody>" << std::endl;
    for (auto &i : bug_stats)
    {
        out << "                <tr>" << std::endl;
        out << "                    <td>" << std::endl;
        out << "                        " << i._bug_type << std::endl;
        out << "                    </td>" << std::endl;
        out << "                    <td>" << std::endl;
        out << "                        " << i.get_num() << std::endl;
        out << "                    </td>" << std::endl;
        out << "                    <td>" << std::endl;
        //Set the precision to two decimal places
        out << "                        " << std::setiosflags(std::ios::fixed) << std::setprecision(2)
            << (double)i.get_num() / (double)StatsHelper::instance()->get_all_bug_num() * 100.0 << "%"
            << std::endl;
        out << "                    </td>" << std::endl;
        out << "                </tr>" << std::endl;
    }
    out << "            </tbody>" << std::endl;
    out << "            <tfoot>" << std::endl;
    out << "                <tr>" << std::endl;
    out << "                <td class=\"rounded-foot-left\"></td>" << std::endl;
    out << "                <td></td>" << std::endl;
    out << "                <td class=\"rounded-foot-right\"></td>" << std::endl;
    out << "                </tr>" << std::endl;
    out << "            </tfoot>" << std::endl;
    out << "        </table>" << std::endl;
    //create a jump button for index html
    out << "        <h2 align=\"center\">" << std::endl;
    out << "            To See More Information About Bug Descriptions" << std::endl;
    out << "        </h2>" << std::endl;
    out << "        <div id=\"button-wrapper\">" << std::endl;
    out << "            <a href=\"./index.html\">" << std::endl;
    out << "                <button class=\"click-button\">" << std::endl;
    out << "                    Clicked Here" << std::endl;
    out << "                </button>" << std::endl;
    out << "            </a>" << std::endl;
    out << "        </div>" << std::endl;
    out << "    </body>" << std::endl;
    out << "</html>" << std::endl;
}
// generate filename's detail html,show the error location and infomation in the source code
void HtmlPrinter::generate_detail_html(std::string filename)
{
    //should not create detail html for unknown header file
    if (filename == "")
    {
        INFO("generate detail html for unknown header file.");
        return;
    }
    // get the output filename from the absolute path filename
    std::string new_filename = filename;
    //replace \ or / or . or : to - ,in order to generate a new filename
    std::replace(new_filename.begin(), new_filename.end(), '/', '-');
    std::replace(new_filename.begin(), new_filename.end(), '\\', '-');
    std::replace(new_filename.begin(), new_filename.end(), '.', '-');
    std::replace(new_filename.begin(), new_filename.end(), ':', '-');
    std::string out_filename = output_path + "/modules/" + new_filename + ".html";
    //create detailed file for this file
    std::ofstream out(out_filename, std::ios::out | std::ios::trunc);
    if (!out.is_open())
    {
        INFO("Error when creating the detail.html file\n");
        return;
    }
    //output header information
    out << "<!DOCTYPE html>" << std::endl;
    out << "<html xmlns=\"http://www.w3.org/1999/xhtml\">" << std::endl;
    out << "    <head>" << std::endl;
    out << "        <meta charset=\"utf-8\"/>" << std::endl;
    out << "        <title>" << filename << "</title>" << std::endl;
    out << "        <link rel=\"stylesheet\" type=\"text/css\" "
           "href=\"../css/prettyprint.css\"/>"
        << std::endl;
    out << "        <script type=\"text/javascript\" "
           "src=\"../js/prettyprint.js\"></script>"
        << std::endl;
    out << "    </head>" << std::endl;
    out << "    <body onload=\"prettyPrint()\">" << std::endl;
    out << "		<pre class=\"prettyprint lang-cpp linenums\">" << std::endl;
    //open source code file
    std::ifstream in(filename, std::ios::in);
    if (!in.is_open())
    {
        INFO("Error when opening the source file:%s\n", filename.c_str());
        return;
    }
    //generate error line nums list for this file
    std::vector<int> error_line_nums;
    auto file_stats = StatsHelper::instance()->get_file_stats_list();
    for (auto &i : file_stats)
    {
        if (i._filename == filename)
        {
            auto bug_stats = i.get_bug_stats_list();
            for (auto &j : bug_stats)
            {
                auto bug_descs = j.get_bug_desc_list();
                for (auto &k : bug_descs)
                {
                    error_line_nums.push_back(k._line_number);
                }
            }
            break;
        }
    }
    std::string line;
    //record line num when read the source code
    int linenum = 0;
    while (std::getline(in, line))
    {
        linenum++;
        int index = 0;
        //must first transfer &, and then transfer < and >, otherwise the & produced 
        //after < and > transferation will be transfered again.
        //transfer & to &amp;
        while (line.find("&", index) != std::string::npos)
        {
            int loc = line.find("&", index);
            std::string sub = line.substr(loc + 1);
            line = line.substr(0, loc + 1);
            line.replace(loc, loc + 1, "&amp;");
            line = line + sub;
            //jump over the transfered "&amp;" to avoid endless loop
            index = loc + 5;
        }
        //transfer < to &lt;
        while (line.find("<") != std::string::npos)
        {
            int loc = line.find("<");
            std::string sub = line.substr(loc + 1);
            line = line.substr(0, loc + 1);
            line.replace(loc, loc + 1, "&lt;");
            line = line + sub;
        }
        //transfer > to &gt;
        while (line.find(">") != std::string::npos)
        {
            int loc = line.find(">");
            std::string sub = line.substr(loc + 1);
            line = line.substr(0, loc + 1);
            line.replace(loc, loc + 1, "&gt;");
            line = line + sub;
        }
        if (std::find(error_line_nums.begin(), error_line_nums.end(), linenum) != error_line_nums.end())
        {
            //If the line number of the line is in line_numbers, it will be marked with red line,
            //and find the bug type and bug description in StatsHelper and output
            out << "<span style = \"background-color:#fdd\">" << line;
            for (auto &i : file_stats)
            {
                if (i._filename == filename)
                {
                    auto bug_stats = i.get_bug_stats_list();
                    for (auto &j : bug_stats)
                    {
                        auto bug_descs = j.get_bug_desc_list();
                        for (auto &k : bug_descs)
                        {
                            if (k._line_number == linenum)
                            {
                                out << "        " << j._bug_type << ":" << k._description;
                                break;
                            }
                        }
                    }
                }
            }
            out << "</span>" << std::endl;
        }
        else
        {
            out << line << std::endl;
        }
    }
    out << "      </pre>" << std::endl;
    out << "    </body>" << std::endl;
    out << "</html>" << std::endl;
    out.close();
    in.close();
}
// print all file
void HtmlPrinter::print()
{
    generate_style_css();
    generate_prettyprint_css();
    generate_prettyprint_js();
    generate_index_html();
    //if statis output options is open, generate stats.css and stats.html
    if (statis)
    {
        generate_stats_css();
        generate_stats_html();
    }
    //Generate detailed html for each source code file where the bug was detected
    auto detail_files = StatsHelper::instance()->get_filename_list();
    for (auto &i : detail_files)
    {
        //when the file is unknown header file, ignore it
        if (i == "")
            continue;
        else
        {
            generate_detail_html(i);
        }
    }
}