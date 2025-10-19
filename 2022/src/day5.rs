use regex::Regex;

fn parse_crates(lines : Vec<&str>) -> Vec::<Vec::<char>> {
  let mut crates = Vec::<Vec::<char>>::new();
  let mut num_crates = 0;

  for (index, line) in lines.iter().rev().enumerate() {
    if(index == 0) {
      num_crates = (line.len() + 1) / 4;
      crates = vec![vec![]; num_crates];
      continue;
    } 

    for cr in (0..num_crates) {
      let c = line.as_bytes()[cr*4 + 1] as char;
      if c != ' ' { crates[cr].push(c); }
    }
  }
  return crates;
}

fn do_moves(lines : Vec<&str>, crates : &mut Vec::<Vec::<char>>, p2 : bool) {
  let re = Regex::new(r"move (\d+) from (\d+) to (\d+)").unwrap();
  for line in lines {
    for cap in re.captures_iter(line) {
      let num  = cap[1].parse::<usize>().expect("failed parsing num"); 
      let from = cap[2].parse::<usize>().expect("failed parsing from") - 1;
      let to   = cap[3].parse::<usize>().expect("failed parsing to") - 1;
      //println!("move {num} from {from} to {to}");

      if !p2 {
        for _i in 0..num {
          if crates[from].is_empty() { break; }
            let c = crates[from].pop().expect("crates[{from}] failed pop");
            crates[to].push(c);
        }
      } else {
        let f_len = crates[from].len();
        let cr: Vec<char> = crates[from].drain(f_len - num..).collect();
        for c in cr {
          crates[to].push(c);
        }
      }
    }
  }
}

fn print_crates_top(stdout: &mut dyn std::io::Write, crates : &Vec::<Vec::<char>>) {
  for cr in crates {
    let c = cr[cr.len() - 1];
    if(c != ' ') {
      write!(stdout, "{c}").expect("write_error");
    }
  }
  writeln!(stdout).expect("write_error");
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let data: Vec<&str> = content.split("\n\n").collect();
  let (crates, moves) = (data[0], data[1]);

  let mut crates1 = parse_crates(crates.split("\n").collect());
  do_moves(moves.split("\n").collect(), &mut crates1, false);
  print_crates_top(stdout, &crates1);

  let mut crates2 = parse_crates(crates.split("\n").collect());
  do_moves(moves.split("\n").collect(), &mut crates2, true);
  print_crates_top(stdout, &crates2);
  
}