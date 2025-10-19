pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let elves = content.split("\n\n");
  
  let mut calorie_sums : Vec<i32> = elves.map(|a| a.split("\n")
  .map(|a| a.parse::<i32>().unwrap()) // convert to int
  .sum()).collect();
  
  calorie_sums.sort();
  writeln!(stdout, "{:?}", calorie_sums[calorie_sums.len()-1]).ok();
  writeln!(stdout, "{:?}", calorie_sums[calorie_sums.len()-3..].iter().sum::<i32>()).ok();
}

