#if 0

#include <cfile.h>
#include <libapp.h>
#include <libpath.h>
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

bool _writeMd(const char *inpath, const char *search)
{
    if (!inpath || !strlen(inpath) || !search || !strlen(search))
        return false;

    CStringAuto *temp = cstr_new(search);
    cstr_replace(temp, " ", "_", true);

    CStringAuto *outpath = cstr_new(inpath);
    path_strip_ext(outpath, true);
    cstr_append_c(outpath, ' ');
    cstr_append(outpath, c_str(temp));
    cstr_append(outpath, ".md");

    CFileAuto *file = cfile_new();
    if (!cfile_read(file, inpath))
        return false;

    CStringAuto *line = cstr_new_size(512);

    CStringAuto *outbuff = cstr_new_size(1024);
    bool header = false;
    CStringAuto *value = cstr_new_size(64);

    while (cfile_getline(file, line))
    {
        char *start = cstr_data(line);
        char *p = NULL;

        while (isspace(*start)) ++start;

        // write header
        if (!header && strncmp(start, DESC, DESC_LEN) == 0)
        {
            start += DESC_LEN;

            if ((start = strchr(start, '>')) == NULL)
                return false;

            ++start;

            const char *end;
            if ((end = strchr(start, '<')) == NULL)
                return false;

            cstr_copy_len(value, start, end - start);

            if (cstr_compare(value, search, true) != 0)
                continue;

            print(c_str(value));

            cstr_append(outbuff, "#### ");
            cstr_append(outbuff, c_str(value));
            cstr_append(outbuff, "\n\n");

            header = true;
        }

        // write link
        else if (header && (p = strstr(start, HREF)) != NULL)
        {
            p += HREF_LEN;

            char *end;
            if ((end = strchr(p, '\"')) == NULL)
                return false;

            cstr_copy_len(value, p, end - p);

            if ((p = strchr(end, '>')) == NULL)
                return false;

            ++p;

            if ((end = strchr(p, '<')) == NULL)
                return false;

            *end = '\0';

            cstr_append(outbuff, "* ");
            cstr_append(outbuff, p);
            cstr_append(outbuff, "\n    \n    ");

            int len = cstr_size(value);
            if (len > MAXLEN)
            {
                char *valcpy = strdup(c_str(value));
                valcpy[MAXLEN] = '\0';

                cstr_append(outbuff, "[");
                cstr_append(outbuff, valcpy);
                cstr_append(outbuff, "](");
                cstr_append(outbuff, c_str(value));
                cstr_append(outbuff, ")\n\n");

                free(valcpy);
            }
            else
            {
                cstr_append(outbuff, c_str(value));
                cstr_append(outbuff, "\n\n");
            }
        }
        else if (header && strncmp(start, END, END_LEN) == 0)
        {
            break;
        }
    }

    if (!file_write_len(c_str(outpath), cstr_data(outbuff), cstr_size(outbuff)))
        return false;

    return true;
}

#endif


