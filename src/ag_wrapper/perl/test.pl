use ag;
$agset = ag::CreateAGSet("aa");
$tl = ag::CreateTimeline($agset);
$ag = ag::CreateAG($agset, $tl);
$x = ag::CreateAnchor($ag);
$y = ag::CreateAnchor($ag);
$ann = ag::CreateAnnotation($ag, $x, $y, "test");
print ag::toXML($agset);
ag::DeleteAGSet($agset);
