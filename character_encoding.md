# Character Encoding 

### What is character encoding and why does it matter?
If you're using characters that aren't standard english letters, they may not show up correctly unless we specify the "character encoding" that we want to use.

In computing, characters and letters are represented by a unique number, called a **codepoints**. Deeper though, characters are stored as bytes, and in order to "translate" those bytes to look like letters, we need to use a character encoding that will map specific bytes to characters within a related group of characters (**character set**). Without using a character encoding, we'd just see binary and data values, rather than human-readable character values.

There have been many different character encoding throughout the years, so as you develop or surf the web, you may hear about and experience various different types of encoding

#### Relating to fonts
A font is a collection of 'glyph' definitions, and glypths are just the shapes that are used to display characters. After an app realizes what characters it's working with, it will now need to render those characters. It looks in the front for the glyphs it can to render those characters.  A font usually covers a character set, meaning it will have glyphs for each character. If your font doesn't have a glyph on your system, hte browser or application you're using will try to find a replacement using the other fonts on your system. Otherwise you'll see a square box, question mark, or some other character instead. 


### Document Character Set and character escapes:
In modern HTML, you are required to use Unicode. You also probably hear about character escapes which is the idea of representing a character without using that character. For example this character escape&#x05D0; represents some hebrew letter.

### History Lesson and Unicode
We first started with an encoding called American Standard Code for Information Interchange **(ASCII)**. This coding used 7 bits to represent 128 different characters. It contained upper and lower case english letters, digits from 0-9, and some special characters. So every number from 0-127 represented some character.

Tech likes to process things as bytes (8 bits), so we started using the 8th bit, which allowed us to represent 256 characters. However the characters represented by numbers 128-255 were never standardized. Different groups and countries started using creating their own code pages (character sets), which meant that the number 129 may represent a certain character in one language but something completely different in another language.

Then comes Unicode, a new standard that would assign a unique number (officially known as a code point) to every letter in every language in the world. In the modern day, that's over 100,000 codepoints. The first 128 Unicdoe code points are the same as ASCII, 128-255 contain currency symbols, and so on and so forth. Now there's no ambiguity. Now this idea would require a lot more than 8 bits to represent that many characters.

I mean computers have become advanced, we have 64 bit processors now. A lot of software in C or C++ has a wide character type, which is a 32 bit character. Modern web browsers use this idea or something similar and can esaily deal with over 4 billion unique characters, so technically modern web browsers use Unicode. This is good, but the crux of this issue lies in the process of sending and receiving these characters:
- A lot of existing software and protocols send/receive/read/write 8 bit character .
- Using 32 bits to represent character would quadruple the amount of bandwidth/space required.

The issue is to find a way to represent 110,000 Unicode code points with just 8 bits. There have been several solutions over the years, but the reigning king is UTF-8 (Universal Character Set Transformation Format 8 bit). And that's about it, it's been the most used, and it should be the one that modern websites and computers use. As long as everyone is using UTF-8, everything should work great: 

### Issues: Blanks, Question Marks, and Boxes:
Even if your browsers up-to-date with UTF-9 and Unicdoe, your browser may still not know how to render a certain character. Remember that Unicdoe defines over 110,000 characters, so it may not have the correct font for all of them. For the more obscure ones, it may not even be able to render it, and show a question makr, blank, or a box with tiny numbers niside of it. This is fairly common in older browsers. 


### How to handle character encoding as a developer?
Choose UTF-8 character encoding. This is a Unicode encoding, and it's pretty good since you can use a single encoding scheme to handle any character ever. Here's what you need to know:
- How to declare a character encoding
- Merely declaring an eocding own't change the bytes, so you'll need to save the text in said encoding
- Check what encoding your editor or scripts are saving it, and try to save those to uTF-8


```html
<meta charset="utf-8">
```
- Use @charset or HTTP headers to declare the encoding of your style sheets. Though thisis only necessary if your style sheet contains non-ASCII characters.
- Try to avoid using byte-order mark in UTF-8 and ensure your HTML code is saved in Unicode normalization form C (NFC).
- Avoid using character escapes, except for invisible or ambiguous characters. Also don't use Unicode control characters hwen you can use markup instead.  






## Credits:
- [What is character encoding](https://www.w3.org/International/questions/qa-what-is-encoding)
- [Handling character encodings in HTML and CSS](https://www.w3.org/International/tutorials/tutorial-char-enc/)
- [Character encoding history](https://www.smashingmagazine.com/2012/06/all-about-unicode-utf8-character-sets/)