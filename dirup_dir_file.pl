#!/usr/bin/perl -w
#Purpose:
#  Find directory of for file with filename=$ARGV[2] 
#  starting from directory=$ARGV[1] and searching
#  upward in directory heirarchy.  Print result to stdout.
#
package main
; my $dir_cur = shift
  || die "$0 needs 1st arg as directory name to start search for filename (2nd arg)"
; my $fname = shift
  || die "$0 needs 2nd arg as file name to search up for starting in directory (2nd arg)"
; $#ARGV==-1 
  || die "only 2 args(not containing blanks) allowed.\n These, '@ARGV', should be removed \n or maybe the blanks replaced with something else."
; my @dir_stk=split("/",$dir_cur);
; my $full_path=$dir_cur . "/" . $fname
#; print "full_path=$full_path\n"
; my $found= (-r $full_path)?1:0
; my $pop_val=pop(@dir_stk)
; while(!$found && $pop_val)
  { $dir_cur=join("/",@dir_stk)
  ; $full_path=$dir_cur . "/" . $fname
  #; print "full_path=$full_path\n"
  ; $found= (-r $full_path)?1:0
  ; $pop_val=pop(@dir_stk)
  ;}
; if(!$found)
  { $dir_cur="/dev/null"
  ;}
; print $dir_cur
;
