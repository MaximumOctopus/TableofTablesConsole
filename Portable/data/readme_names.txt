
ToT has a powerful name generating system.

Data files for generating names can be found in \names\


========================================================================
========================================================================
== Names ===============================================================
========================================================================
========================================================================


The format for a \names\ data file is as follows:

    {
    d:NAMES
    i:A list of names
    i:The order does not matter
    d:ADJECTIVES
    i:A list of adjectives
    i:The order does not matter
    d:NOUNS
    i:A list of nouns
    i:The order does not matter
    d:TITLES
    i:A list of titles / salutations
    i:The order does not matter
    d:OTHER
    i:A list of custom text entries
    i:The order does not matter
    d:TEMPLATES
    i:A list of name templates, see below
    }


========================================================================
== Name templates ======================================================
========================================================================


A names data file can have as many templates as needed, these define
how names will be structured within the application.

$title     = outputs a random title/salutation
$name      = outputs a random name
$adjective = outputs a random adjective
$noun      = outputs a random noun
$other     = outputs a random "other"

Templates can contain any other words or characters. Only tokens prefixed
by $ (above) will be replaced with randomly selected words.

Exampls (from the generic.txt name file)

    $title $name $adjective $noun

    $name $adjective $noun

    $title $name $noun

    $name $noun

    $title $name the $adjective

    $name the $adjective


To generate a random name within any table item, use $ plus the name of
the name file: $generic for the generic.txt name generator.


========================================================================
==                                                                    ==
========================================================================

- Paul A Freshney, August 17th 2017
  paul@freshney.org

========================================================================
==                                                                    ==
========================================================================