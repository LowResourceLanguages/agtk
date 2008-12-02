#! /usr/bin/perl
# swig interface generator for AGAPI.h & agtree.h
# Haejoong Lee (haejoong@ldc.upenn.edu)

while (<>) {
  s/^[ \t\n]*//;
  if (!$_) { next; }
  if (/^using/) { next; }
  if (/^[^a-zA-Z]/) { next; }
  s/DllExport //;
  unless (/;[ \t]*$/) { chomp; $line.=$_." "; next; }
  $_ = $line . $_; $line = "";
  s/[ \t]*$//;
  s/[ \t]+/ /g;
  s/[ \t]*,[ \t]*/,/g;
  s/[ \t]*=[ \t]*/=/g;
  s/ [^ \t=,]+(,|\))/\1/g;
  s/ [a-zA-Z0-9]+=/=/g;

  s/\bString\b/string/g;
  s/\bId\b/string/g;
  s/\bAGSetId\b/string/g;
  s/\bAGId\b/string/g;
  s/\bAGIds\b/string/g;
  s/\bAnnotationId\b/string/g;
  s/\bAnnotationType\b/string/g;
  s/\bAnnotationIds\b/string/g;
  s/\bAnchorId\b/string/g;
  s/\bAnchorIds\b/string/g;
  s/\bTimelineId\b/string/g;
  s/\bSignalId\b/string/g;
  s/\bSignalIds\b/string/g;
  s/\bFeatureName\b/string/g;
  s/\bFeatureNames\b/string/g;
  s/\bFeatureValue\b/string/g;
  s/\bFeatures\b/string/g;
  s/\bURI\b/string/g;
  s/\bMimeClass\b/string/g;
  s/\bMimeType\b/string/g;
  s/\bEncoding\b/string/g;
  s/\bUnit\b/string/g;
  s/\bTrack\b/string/g;
  s/\bAnnotationRef\b/string/g;
  s/\bOffset\b/double/g;

  ($head, $_, $tail) = /^([^(]*\()([^)]*)(\).*)$/;
  @args = ();
  for (split /,/) {
    if (/>/ && !/</) {
      $temp = pop @args;
      push @args, "$temp,$_";
    } else {
      push @args, $_;
    }
  }
  while (@args >= 0) {
    $flag = 0;
    $output = "";
    for (@args) {
      $temp = $_;
      $flag = 1 if $temp =~ s/=.*//;
      $output .= "$temp,";
    }
    chop $output;
    print $head . $output . $tail . "\n";
    last unless $flag;
    pop @args;
  }

  #print "extern $_";
}

