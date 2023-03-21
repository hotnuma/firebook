#include <cfile.h>
#include <libpath.h>

#include <gumbo.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <print.h>

#define MAXLEN 60

const char* node_gettext(GumboNode *node)
{
    if (node->v.element.children.length != 1)
        return NULL;

    GumboNode* title_text = node->v.element.children.data[0];

    if (title_text->type != GUMBO_NODE_TEXT
        && title_text->type != GUMBO_NODE_WHITESPACE)
        return NULL;

    return title_text->v.text.text;
}

bool node_getheader(GumboNode *node, const char **outtext)
{
    if (node->v.element.tag != GUMBO_TAG_H3)
        return false;

    *outtext = NULL;

    const char *text = node_gettext(node);
    if (!text)
        return false;
    *outtext = text;

    return true;
}

bool node_getlink(GumboNode *node, const char **outtext, const char **outlink)
{
    if (node->v.element.tag != GUMBO_TAG_A)
        return false;

    *outtext = NULL;
    *outlink = NULL;

    GumboAttribute *href = gumbo_get_attribute(&node->v.element.attributes, "href");
    if (!href)
        return false;
    *outlink = href->value;

    const char *text = node_gettext(node);
    if (!text)
        return false;
    *outtext = text;

    return true;
}

static void node_search(GumboNode *node, CFile *outfile, const char *title)
{
    if (node->type != GUMBO_NODE_ELEMENT)
        return;

    static int level = 0;
    static int current = -1;

    const char *text;
    const char *link;

    if (node_getheader(node, &text))
    {
        if (level <= current)
        {
            current = -1;
            return;
        }

        if (strcmp(text, title) == 0)
        {
            print("found : %s", text);

            cfile_writefmt(outfile, "#### %s\n\n", text);

            current = level;
            return;
        }

        return;
    }

    else if (node_getlink(node, &text, &link))
    {
        if (current != -1 && level > current)
        {
            //print("%d : %s %s", level, text, link);

            cfile_writefmt(outfile, "* %s\n    \n    ", text);

            int len = strlen(link);
            if (len > MAXLEN)
            {
                char *valcpy = strdup(link);
                valcpy[MAXLEN] = '\0';

                cfile_writefmt(outfile, "[%s](%s)  \n\n", valcpy, link);

                free(valcpy);
            }
            else
            {
                cfile_writefmt(outfile, "%s  \n\n", link);
            }
        }

        return;
    }

    ++level;

    GumboVector *children = &node->v.element.children;

    for (unsigned int i = 0; i < children->length; ++i)
    {
        node_search((GumboNode*) children->data[i], outfile, title);
    }

    --level;
}

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

    GumboOutput *output = gumbo_parse(cfile_data(file));

    if (!cfile_open(file, c_str(outpath), "wb"))
    {
        gumbo_destroy_output(&kGumboDefaultOptions, output);
        return false;
    }

    node_search(output->root, file, search);

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    fflush(stdout);

    return true;
}

static int _app_exit(bool usage, int ret)
{
    if (usage)
    {
        print("*** usage : firebook -i bookmark.html \"Menu Title\"");
    }

    return ret;
}

int main(int argc, char **argv)
{
    const char *inpath = NULL;
    const char *search = NULL;

    if (argc < 2)
        return _app_exit(true, EXIT_FAILURE);

    int n = 1;
    while (n < argc)
    {
        const char *part = argv[n];

        // Input file.
        if (strcmp(part, "-i") == 0)
        {
            if (++n >= argc)
                return EXIT_FAILURE;

            inpath = argv[n];
        }
        else
        {
            if (search != NULL)
                return EXIT_FAILURE;

            search = argv[n];
        }

        ++n;
    }

    if (!_writeMd(inpath, search))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


