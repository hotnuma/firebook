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

bool _writeHtml(const CString &inpath, const CString &search);
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

bool _writeHtml(const CString &inpath, const CString &search)
{
    CString temp = search;
    temp.replace(" ", "_");

    CString outpath = inpath;
    pathStripExt(outpath);
    outpath += strFmt("_%s.html", temp.c_str());

    CFile file;
    if (!file.read(inpath))
        return 1;

    CString outbuff;

    outbuff += "<!DOCTYPE NETSCAPE-Bookmark-file-1>\n";
    outbuff += "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=UTF-8\">\n";
    outbuff += "<TITLE>Bookmarks</TITLE>\n";
    outbuff += "<H1>Menu des marque-pages</H1>\n";
    outbuff += "\n";
    outbuff += "<DL><p>\n";

    CString line;
    bool wline = false;

    while (file.getLine(line))
    {
        const char *p = line.c_str();

        while (isspace(*p)) ++p;

        if (line.startsWith(DESC))
        {
            p += strlen(DESC);

            if ((p = strchr(p, '>')) == nullptr)
                return false;

            ++p;

            const char *end;
            if ((end = strchr(p, '<')) == nullptr)
                return false;

            CString desc;
            desc.append(p, end - p);

            if (desc == search)
            {
                print(desc.c_str());

                outbuff += line;
                outbuff += "\n";

                wline = true;
            }
        }
        else if (wline && line == END)
        {
            outbuff += line;
            outbuff += "\n";

            wline = false;
        }
        else if (wline)
        {
            outbuff += line;
            outbuff += "\n";
        }
    }

    outbuff += "</DL>\n";

    if (!CFile::write(outpath, outbuff))
        return 1;

    return true;
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
            outbuff += "\n";

            header = true;
        }
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

            outbuff += "[";
            outbuff += p;
            outbuff += "](";
            outbuff += value;
            outbuff += ")\n";
        }
        else if (header && line == END)
        {
            break;
        }
    }

    if (!CFile::write(outpath, outbuff))
        return 1;

    return true;
}


