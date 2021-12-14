#if 0

bool _writeHtml(const CString &inpath, const CString &search);

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

#endif


