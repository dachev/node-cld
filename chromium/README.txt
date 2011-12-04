10/21/2011

This package contains the CLD (Compact Language Detection) library,
extracted from the source code for Google's Chromium library at
http://src.chromium.org/svn/trunk/src/third_party/cld
(http://src.chromium.org/viewvc/chrome/trunk/src/third_party/cld),
specifically revision 105735.

There are two components:

  * A standalone C++ library (libcld.a), which you can use
    from any C++ program.

  * Simple Python bindings around that library.

There is also a Python unit test, ported from the unit test from CLD,
verifying that the library identifies languages correctly and also
showing how to use it.

The LICENSE is the same as Chromium's LICENSE.

This was a very simple, fast rote port: I just extracted the
referenced C++ sources from the cld.gyp sources, translated to
build.sh / setup.py, and got things to compile / pass the test.  I
removed one source file
(encodings/compact_lang_det/win/cld_unicodetext.cc): it wraps CLD,
adding a utility method to convert from UTF16 to UTF8, and normalize
text using ICU.  This means such conversion and normalizing will have
to be done by the apps using this library.  Otherwise I made no
changes to Chromium's sources.

Much more can be done, eg build a dynamic library too, use "make" to
compile everything, expose more of the API to Python,
simplify/refactor the code, etc.

I have only tested this on Fedora 13 with Python 2.6.4; it passes all
tests in test.py there (python -u test.py).

There is also a simple example.cc showing basic usage from C++ code.

Finally: I'd like to thank Google and the Chromium team and the
original Google toolbar authors for 1) creating CLD in the first
place, 2) open-sourcing it, and 3) choosing a generous LICENSE for the
source code.

Your ideas will go further if you don't insist on going with them!



USING THE PYTHON LIBRARY

Once you've compiled & installed the Python bindings (see
INSTALL.txt), detection is easy.

First, you must get your content (plain text or HTML) encoded into
UTF8 bytes.  Then, detect like this:

  topLanguageName, topLanguageCode, isReliable, textBytesFound, details = cld.detect(bytes)

The code and name of the top language is returned.  isReliable is True
if the top language is much better than 2nd best language.
textBytesFound tells you how many actual bytes CLD analyzed (after
removing HTML tags, collapsing areas of too-many-spaces, etc.).
details has an entry per top 3 languages that matched, that includes
the percent confidence of the match as well as a separate normalized
score.

The detect method takes optional params:

  * isPlainText (default is False): set to True if you know your bytes
    don't have any XML/HTML markup

  * includeExtendedLanguages (default is True): set to False to
    exclude "extended" languages added by Google

  * hintTopLevelDomain (default is None): set to the last part of the
    domain name that the content came from (for example if the URL was
    http://www.krasnahora.cz, pass the string 'cz').  This gives a
    hint that can bias the detector somewhat.

  * hintLanguageCode (default is None): set to the possible language.
    For example, if the web-server declared the language, or the
    content itself embedded an http-equiv meta tag declaring the
    language, pass this (for example, "it" for Italian).  This gives a
    hint that can bias the detector somewhat.

  * hintEncoding (default is None): set to the original encoding of
    the content (note you still must pass UTF-8 encoded bytes).  This
    gives a hint that can bias the detector somewhat.  NOTE: this is
    currently not working.

  * pickSummaryLanguage (default is False): if False, CLD will always
    return the top matching language as the answer.  If True, it will
    sometimes pick 2nd or 3rd match (for example, if English and X
    match, where X (not UNK) is big enough, assume the English is
    boilerplate and return X).  In simple testing accuracy seems to
    suffer a bit (XX to YY %) when this is True so I've defaulted to
    False.

  * removeWeakMatches (default is True): if a match isn't strong
    enough, delete it.  This ensures some amount of confidence when a
    language is returned.


The module exports these global constants:

  * cld.ENCODINGS: list of the encoding names CLD recognizes (if you
    provide hintEncoding, it must be one of these names).

  * cld.LANGUAGES: list of languages and their codes (if you provide
    hintLanguageCode, it must be one of the codes from these codes).

  * cld.EXTERNAL_LANGUAGES: list of external languages and their
    codes.  Note that external languages cannot be hinted, but may be
    matched if you pass includeExtendedLanguages=True (the default).

  * cld.DETECTED_LANGUAGES: list of all detectable languages, as best
    I can determine (this was reverse engineered from a unit test, ie
    it contains a language X if that language was tested and passes
    for at least one example text).
