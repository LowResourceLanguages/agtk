#! /usr/bin/env perl

while (<>) {
    if (/\{\"ag::LoadFromDB\", _wrap_LoadFromDB\},/) {
        print "#ifdef AGDB\n";
    }
    print;
    if (/if \(items ==/) {
        $count = 0;
        while (<>) {
            /\{/ and $count++;
            /\}/ and $count--;
            if (/^\s*\(\*PL_markstack_ptr\+\+\);SWIG_CALLXS\(_wrap_/) {
                print;
                last;
            }
        }
        while ($count-- > 0) { <>; }
        next;
    }
    if (/\{\"ag::StoreSQLs\", _wrap_StoreSQLs\},/) {
        print "#endif\n";
    }
}
            
