# AGSBMFontRenderer

The AGS Plugin for display UTF-8 text with Bitmap Font.

### require

- AGS 3.3.3+
- [AngelCode's BMFont (Bitmap Font Generator)](http://www.angelcode.com/products/bmfont/)
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


## NOTE

AngelCode's BMFont supports select characters from UTF-8/UTF-16 text file. this file must be **WITH**-bom.

----

### Download

download AGSBMFontRenderer.dll from a latest successfull build's artifact.
check [build/artifacts](https://ci.appveyor.com/project/bggd/agsbmfontrenderer/build/artifacts)
 or [history](https://ci.appveyor.com/project/bggd/agsbmfontrenderer/history).

### Script API

```
void SetBMFont(String file, int fontNumber);
```

### Example

add ```your_font.fnt``` and ```your_font0.tga``` to Compiled folder.

create [New Font] from [Fonts] in AGS Editor. 
change it propertis:

- OutlineFont for self id.
- OutlineStyle for automatic.

edit ```room1.asc``` with text editor(**Not AGS's text editor**). then save to UTF-8 **WITHOUT**-bom.

```
// room script file

function room_FirstLoad()
{
  SetBMFont("unifont.fnt", 3);
  Game.SpeechFont = 3;
}

function room_AfterFadeIn()
{
  cEgo.Say("外国語の学習と教授 Language Learning and Teaching Изучение и обучение иностранных языков Tere 語文教學・语文教学 ქართული ენის შესწავლა და სწავლება 말배우기와 가르치기 Γλωσσική Εκμὰθηση και Διδασκαλία");
}
```

![screenshot](https://pbs.twimg.com/media/CFxT9pRUUAAjUiY.png)

# License

AGSBMFontRenderer is licensed under Boost Software License v1.0

# Third Party License

AGSBMFontRenderer is using some library. these library has own license. see official repository.

- agsplugin.h https://github.com/adventuregamestudio/ags
- stb_image.h https://github.com/nothings/stb
- utf8-cpp http://sourceforge.net/projects/utfcpp/
