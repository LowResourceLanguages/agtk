#! /usr/bin/perl
# fix jagJNI.java (swig-generated file)
# Haejoong Lee (haejoong@ldc.upenn.edu)

while (<>) {
  print;
  next unless /^.*[ \t]+([^ \t]+)[ \t]+([^ \t]+)(__SWIG_[0-9]+)(.*);[ \t]*$/;
  $rtype = $1;
  $fname = $2;
  $args = $4;
  $swig_fname = $2 . $3;
  print "  public static $rtype $fname$args {\n"; 
  $args =~ s/,[ \t]*[^ \t)]*[ \t]*/, /g;
  $args =~ s/\([ \t]*[^ \t)]*[ \t]*/(/;
  if ($rtype eq "void") {
    print "    ";
  } else {
    print "    return ";
  }
  print "$swig_fname$args;\n";
  print "  }\n";
}

