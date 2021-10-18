#include "CFile.h"
#include "libfile.h"
#include "libapp.h"
#include "libpath.h"
#include <string.h>

#include "print.h"

#define DESC "        <DT><H3 "
#define END "        </DL><p>"

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

    CString temp = search;
    temp.replace(" ", "_");

    CString outpath = strFmt("%s_%s.html",
                             pathBaseName(inpath).c_str(), temp.c_str());

//    CString buffer;

//    if (!fileRead(inpath, buffer))
//        return 1;

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

    //char *ptr = buffer.data();

    CString line;
    bool wline = false;

    while (file.getLine(line))
    {
        const char *p = line.c_str();

        if (line.startsWith(DESC))
        {
            p += strlen(DESC);

            if ((p = strchr(p, '>')) == nullptr)
                return 1;

            ++p;

            const char *end;
            if ((end = strchr(p, '<')) == nullptr)
                return 1;

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

    //if (!fileWrite(outpath, outbuff))

    if (!CFile::write(outpath, outbuff))
        return 1;

    return 0;
}


