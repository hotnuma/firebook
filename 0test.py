import os

basedir = os.path.dirname(__file__)

firebook = os.path.join(basedir, "build\\firebook.exe")
inpath = os.path.join(basedir, "tests\\bookmarks.html")
search = "Fav 1"

cmd = "%s -i \"%s\" \"%s\"" % (firebook, inpath, search)

print(cmd)

os.system(cmd)


