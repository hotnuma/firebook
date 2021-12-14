#### Firebook

Exports a submenu from bookmark.html file into a md file.

#### Install

```bash    
git clone https://github.com/hotnuma/firebook.git
cd firebook
./install.sh
```

#### Usage

* Export "Misc" submenu into Misc.md
    
    ```bash
    firebook -i bookmark.html "Misc"
    ```

* Export "Misc" submenu into Misc.html
    
    ```bash
    firebook -i bookmarks.html "Misc" && mdconv bookmarks_Misc.md
    ```
    
    
