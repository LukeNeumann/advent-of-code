fn compute(lines : std::slice::Iter<&str>, part2: bool) -> u32 {
  let mut count = 0;

  for line in lines {
    let elves: Vec<&str> = line.split(",").collect();
    let sections1: Vec<u32> = elves[0].split("-").map(|a| a.parse::<u32>().unwrap()).collect();
    let sections2: Vec<u32> = elves[1].split("-").map(|a| a.parse::<u32>().unwrap()).collect();
    let (start1,end1) = (sections1[0], sections1[1]);
    let (start2,end2) = (sections2[0], sections2[1]);

    if(start1 <= start2 && end1 >= end2) || (start1 >= start2 && end1 <= end2) || 
       part2 && ((start1 <= start2 && end1 >= start2) || (start2 <= start1 && end2 >= start1)) {
      //println!("{:?},{:?},{:?},{:?}", start1, end1, start2, end2);
      count += 1;
    } 
  }

  return count;
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let lines : Vec<&str> = content.split("\n").collect();
  


  writeln!(stdout, "{:?}", compute(lines.iter(), false)).ok();
  writeln!(stdout, "{:?}", compute(lines.iter(), true)).ok();
}