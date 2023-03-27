### Firebook

This program converts a bookmark.html file exported from Firefox into a markdown file.

Then it's possible to reorganize links into markdown documents such as these :

https://github.com/hotnuma/doclinux  

And then using a markdown converter, generate html files again and add bookmarks to these into Firefox :-D



#### Usage

In Firefox export all bookmarks using Ctrl+Shift+O and then export to html.

Then convert boolmarks into a md file.

* Export all bookmarks into bookmarks.md
    
    ```
    firebook -i bookmarks.html
    ```

* Export "Misc" submenu into bookmarks_Misc.md
    
    ```
    firebook -i bookmarks.html "Misc"
    ```

* Convert markdown to html

    Using hoedown available here :
    
    https://github.com/hoedown/hoedown  
    
    And `mdconv` script available here :
    
    https://github.com/hotnuma/systools  
    https://github.com/hotnuma/systools/blob/master/src/mdconv  
    
    ```
    mdconv bookmarks_Misc.md
    ```
    
    It's possible also to convert all files in a directory, it's possible to add a command in Geany editor to do so.
    


#### Install Firebook

* Build and install TinyC library

    https://github.com/hotnuma/libtinyc  
    https://github.com/hotnuma/libtinyc/blob/master/Readme.md  

* Install Google's Gumbo Parser
    
    Under Ubuntu :
    
    ```
    sudo apt install libgumbo-dev
    ```

* Install Firebook

    ```
    git clone https://github.com/hotnuma/firebook.git
    cd firebook
    ./install.sh
    ```

    
