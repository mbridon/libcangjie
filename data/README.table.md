These are some details about the Cangjie tables provided with libcangjie.

Legal
=====

The data in the Cangjie table source files is hereby placed in the Public
Domain, in the hope that is is useful.

Sources
=======

This data was generated out of a few sources:

* The Unihan database from the Unicode consortium
* The Unicode specification
* The data from Wan Leung Wong's original libcangjie
* Some careful thought from the libcangjie team

Some of us are not native Chinese speakers. As such, the data might be
incomplete, or even incorrect.

If you find that this is the case, please help us making it better by
[filing bugs](https://github.com/Cangjians/libcangjie/issues).

Data Format
===========

There is one table source file for each version of Cangjie supported in
libcangjie. Currently, that means:

* Cangjie 3 data is in `table-cj3.txt`
* Cangjie 5 data is in `table-cj5.txt`

The format of each table source file is as follows:

1. The actual character the user desires to input.

2. The Simplified Chinese equivalent of the actual character. For example, for
   the line where the actual character is "後", this field should be "后".

3. A boolean (0 or 1) representing whether the character is a Chinese
   character.

4. A boolean representing whether the character exists in the Big5 encoding.

5. A boolean representing whether the character is part of the Hong Kong
   Supplemental Character Set (HKSCS).

6. A boolean representing whether the character is part of the Taiwanese
   Bopomofo/Zhuyin alphabet.

7. A boolean representing whether the character is part of the Japanese Kanji
   alphabet.

8. A boolean representing whether the character is part of the Japanese
   Hiragana alphabet.

9. A boolean representing whether the character is part of the Japanese
   Katakana alphabet.

10. A boolean representing whether the character is a punctuation mark.

11. A boolean representing whether the character is a miscellaneous symbol.

12. The orientation of the character. It can be either 'H', if this character
    is to be used exclusively when writing Chinese horizontally; 'V', if the
    character is to be used exclusively when writing Chinese vertically; or an
    empty value, if the character does not have different horizontal and
    vertical presentations, and therefore should be used in both.

13. A Cangjie code for the character in the Cangjie version corresponding to
    the table source file. When a character has multiple Cangjie codes, each
    will be on a different line.

14. A short input code for the character, or the empty string. This is mostly
    to provide a convenient shortcut for users to quickly input punctuation
    marks and/or symbols (e.g the user can type "," to get '、').

15. An integer corresponding to the desired ordering of the characters.
    Depending on the situation, it can be one of three things:

    1. For characters which were part of the Big5 encoding, this value is
       based on their order in that encoding. It should be the same ordering
       as with the Cangjie input method on a popular operating system.

    2. For characters which have a short input code (see above), this value is
       based on the ordering that the libcangjie authors thought would be
       appropriate.

    3. For all other characters, it is 0.
