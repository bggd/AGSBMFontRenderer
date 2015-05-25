# AGSBMFontRenderer

The AGS Plugin for display UTF-8 text with Bitmap Font.

### require

- AGS 3.3.3+
- AngelCode's BMFont (Bitmap Font Generator)
- UTF-8 supported text editor. **Note: AGS's text editor hasn't support UTF-8)

### current limitations

**Text:**

- UTF-8 **WITHOUT**-bom only

**BMFont:**

- text format (.fnt) only
- 8bit texture only

### recommend font settings of BMFont

- enable Match char height
- disable Font smoothing


##NOTE

AngelCode's BMFont supports select characters from UTF-8/UTF-16 text file. this file must be **WITH**-bom

# License

AGSBMFontRenderer is licensed under Boost Software License v1.0

# Third Party License

AGSBMFontRenderer is using some library. these library has own license. see official repository.

- agsplugin.h https://github.com/adventuregamestudio/ags
- stb_image.h https://github.com/nothings/stb
- utf8-cpp http://sourceforge.net/projects/utfcpp/
