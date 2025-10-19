fn simulate(program: &Vec<(&str,i32)>, registers: &mut Vec<i32>) {
  let mut cycle = 0;
  for (op, arg) in program {
    let _ = match op {
      &"noop" => { 
        cycle += 1; 
        registers.push(registers[cycle-1]);
      },
      &"addx" => { 
        cycle += 2;
        registers.push(registers[cycle-2]);
        registers.push(registers[cycle-2] + arg);
      },
      _ => {}
    };
  }
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let lines = content.split("\n");

  let mut registers: Vec<i32> = vec![1; 1];
  let program: Vec<(&str,i32)> = lines.map(|line| {
    let line_split: Vec<&str> = line.split(" ").collect();
    let op = line_split[0];
    let arg: i32 = if line_split.len() > 1 {line_split[1].parse().expect("failed parsing number")} else {0};
    (op, arg)
  }).collect();

  simulate(&program, &mut registers);

  let mut result = 0;
  for cycle in (19..registers.len()).step_by(40) {
    result += (cycle as i32+1) * registers[cycle];
  }
  
  writeln!(stdout, "{result}").ok();

  for cycle in 0..240 {
    let col = cycle % 40;
    let lit = (registers[cycle] - col as i32).abs() <= 1;

    let p = if lit {'#'} else {'.'};
    write!(stdout, "{p}").ok();
    if col == 39 { writeln!(stdout).ok(); }
  }
}