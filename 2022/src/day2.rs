fn score_round_p1(line : &str) -> u32{
  let moves : Vec<&str> = line.split(" ").map(|a| match a {
      "A"|"X" => "R", 
      "B"|"Y" => "P", 
      "C"|"Z" => "S",
      _ => "NA"
  }).collect();
      
  let mut score = match moves[1] {
      "R" => 1,
      "P" => 2,
      "S" => 3,
      _ => 0
  };

  if moves[0] == moves[1] {
      score += 3;
  } else if (moves[1] == "R" && moves[0] == "S") ||
            (moves[1] == "P" && moves[0] == "R") ||
            (moves[1] == "S" && moves[0] == "P") {
      score += 6;
  }

  return score;
}

fn score_round_p2(line : &str) -> u32{
  let moves : Vec<&str> = line.split(" ").map(|a| match a {
      "A" => "R", 
      "B" => "P", 
      "C" => "S",
      _ => a
  }).collect();

  let mut score = match moves[1] {
      "X" => 0,
      "Y" => 3,
      "Z" => 6,
      _ => 0
  };

  score += match (moves[0], moves[1]) {
      ("R","Y")|("P","X")|("S","Z") => 1,
      ("R","Z")|("P","Y")|("S","X") => 2,
      ("R","X")|("P","Z")|("S","Y") => 3,
      _ => 0
  };

  return score;
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let rounds : Vec<&str> = content.split("\n").collect();
  
  let scores1 = rounds.iter().map(|a| score_round_p1(a));
  let scores2 = rounds.iter().map(|a| score_round_p2(a));
  
  writeln!(stdout, "{:?}", scores1.sum::<u32>()).ok();
  writeln!(stdout, "{:?}", scores2.sum::<u32>()).ok();
}