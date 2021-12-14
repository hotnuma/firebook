#include "CFile.h"
#include "libapp.h"
#include "libpath.h"
#include <ctype.h>
#include <string.h>

#include <print.h>

#define DESC        "<DT><H3 "
#define DESC_LEN    8
#define END         "</DL><p>"
#define END_LEN     8
#define HREF        "<DT><A HREF=\""
#define HREF_LEN    13

#define MAXLEN      60

bool _writeMd(const CString &inpath, const CString &search);

int main(int argc, char **argv)
{
    CString inpath;
    CString search;

    int n = 1;
    while (n < argc)
    {
        CString part = argv[n];

        // Input file.
        if (part == "-i")
        {
            if (++n >= argc)
                return 1;

            inpath = argv[n];
        }
        else
        {
            if (!search.isEmpty())
                return 1;

            search = argv[n];
        }

        n++;
    }

    if (inpath.isEmpty() || search.isEmpty())
        return 0;

    if (!_writeMd(inpath, search))
        return 1;

    return 0;
}

bool _writeMd(const CString &inpath, const CString &search)
{
    CString temp = search;
    temp.replace(" ", "_");

    CString outpath = inpath;
    pathStripExt(outpath);
    outpath += strFmt("_%s.md", temp.c_str());

    CFile file;
    if (!file.read(inpath))
        return 1;

    CString outbuff;

    CString line;
    CString value;
    bool header = false;

    while (file.getLine(line))
    {
        char *start = line.data();
        char *p = nullptr;

        while (isspace(*start)) ++start;

        // write header
        if (!header && strncmp(start, DESC, DESC_LEN) == 0)
        {
            start += DESC_LEN;

            if ((start = strchr(start, '>')) == nullptr)
                return false;

            ++start;

            const char *end;
            if ((end = strchr(start, '<')) == nullptr)
                return false;

            value.clear();
            value.append(start, end - start);

            if (value != search)
                continue;

            print(value.c_str());

            outbuff += "### ";
            outbuff += value;
            outbuff += "\n\n";

            header = true;
        }

        // write link
        else if (header && (p = strstr(start, HREF)) != nullptr)
        {
            p += HREF_LEN;

            char *end;
            if ((end = strchr(p, '\"')) == nullptr)
                return false;

            value.clear();
            value.append(p, end - p);

            if ((p = strchr(end, '>')) == nullptr)
                return false;

            ++p;

            if ((end = strchr(p, '<')) == nullptr)
                return false;

            *end = '\0';

            outbuff += "* ";
            outbuff += p;
            outbuff += "\n    \n    ";

            int len = strlen(value);
            if (len > MAXLEN)
            {
                char *valcpy = strdup(value);
                valcpy[MAXLEN] = '\0';

                outbuff += "[";
                outbuff += valcpy;
                outbuff += "](";
                outbuff += value;
                outbuff += ")  \n\n";

                free(valcpy);
            }
            else
            {
                outbuff += value;
                outbuff += "  \n\n";
            }
        }
        else if (header && line == END)
        {
            break;
        }
    }

    //outbuff += "\n";

    if (!CFile::write(outpath, outbuff))
        return 1;

    return true;
}


