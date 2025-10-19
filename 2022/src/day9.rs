use std::collections::HashMap;

fn simulate(moves: &Vec<(char,u32)>, knots: usize, tail_locations: &mut HashMap::<(i32,i32), bool>) {
  let mut x:Vec<i32> = vec![0; knots];
  let mut y:Vec<i32> = vec![0; knots];

  tail_locations.insert((0,0), true);

  for (dir, num) in moves {
    // println!("== {dir} {num} ==");

    for _ in 0..*num {
      let _ = match dir {
        'U' => { y[0] += 1; },
        'D' => { y[0] -= 1; },
        'L' => { x[0] -= 1; },
        'R' => { x[0] += 1; },
        _ => ()
      };

      for i in 1..knots{
        let dx = x[i-1] - x[i];
        let dy = y[i-1] - y[i];

        let incrx = if dx > 0 {1} else if dx < 0 {-1} else {0};
        let incry = if dy > 0 {1} else if dy < 0 {-1} else {0};
        if dx.abs() > 1 || dy.abs() > 1{
          x[i] += incrx;
          y[i] += incry;
        }
      }

      // let mut xmin = x.iter().min().expect("err min"); if xmin > &0 { xmin = &0; } 
      // let mut xmax = x.iter().max().expect("err max"); if xmax < &0 { xmax = &0; }
      // let mut ymin = y.iter().min().expect("err min"); if ymin > &0 { ymin = &0; }
      // let mut ymax = y.iter().max().expect("err max"); if ymax < &0 { ymax = &0; }
      // for j in (*ymin..=*ymax).rev() {
      //   for i in *xmin..=*xmax {
      //     let mut p = ".".to_string();
      //     if i == 0 && j == 0 { 
      //       p = "s".to_string(); 
      //     } else {
      //       for (ind, _) in x.iter().enumerate() {
      //         if(x[ind] == i && y[ind] == j){
      //           p = if ind == 0 {"H".to_string()} else {ind.to_string()};
      //           break;
      //         }
      //       }
      //     }

      //     print!("{p}")
      //   }
      //   println!();
      // }
      // println!();

      tail_locations.insert((x[knots-1],y[knots-1]), true);
    }
  }
}


pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let lines = content.split("\n");
  let moves: Vec<(char,u32)> = lines.map(|a| {
    let line_split: Vec<&str> = a.split(" ").collect();
    let dir = line_split[0].chars().next().expect("failed parsing direction");
    let num: u32 = line_split[1].parse().expect("failed parsing number");
    (dir, num)
  }).collect();

  let mut tail_locations = HashMap::<(i32,i32), bool>::new();
  simulate(&moves, 2, &mut tail_locations);
  let mut tail_locations2 = HashMap::<(i32,i32), bool>::new();
  simulate(&moves, 10, &mut tail_locations2);
  
  let visit_count = tail_locations.keys().len();
  let visit_count2 = tail_locations2.keys().len();

  writeln!(stdout, "{visit_count}").ok();
  writeln!(stdout, "{visit_count2}").ok();
}