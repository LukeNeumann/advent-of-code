fn is_visible(treeline: &Vec<&u32>, position: usize) -> bool {
  if(position == 0) {
    true
  } else {
    match treeline[0..position].iter().rev().find(|t| t >= &&treeline[position]) {
      Some(_) => false,
      None => true
    }
  }
}

fn get_view(treeline: &Vec<&u32>, position: usize) -> u32 {
  match treeline[0..position].iter().rev().position(|p| p >= &&treeline[position]) {
    Some(p) => p as u32 + 1,
    None => position as u32
  }
}

fn calc_visibility_score(forest: &Vec<Vec<u32>>) -> Vec<Vec<(bool,bool,bool,bool,u32)>> {
  let mut result = vec![vec![(true,true,true,true,0); forest[0].len()]; forest.len()];

  for (i, row) in forest.iter().enumerate() {
    for (j, _tree) in row.iter().enumerate() {
      let row_top = forest.iter().map(|a_row| &a_row[j]).collect::<Vec<&u32>>(); 
      let row_right = row.iter().rev().collect::<Vec<&u32>>(); 
      let row_bot = forest.iter().map(|a_row| &a_row[j]).rev().collect::<Vec<&u32>>();  
      let row_left = row.iter().collect::<Vec<&u32>>(); 
      
      result[i][j].0 = is_visible(&row_top, i); // top
      result[i][j].1 = is_visible(&row_right, row_right.len()-1 - j); // right
      result[i][j].2 = is_visible(&row_bot, row_bot.len()-1 - i); // bottom
      result[i][j].3 = is_visible(&row_left, j); // left
      
      result[i][j].4 = 
        get_view(&row_top, i) *
        get_view(&row_right, row_right.len()-1 - j) * 
        get_view(&row_bot, row_bot.len()-1 - i) *
        get_view(&row_left, j);
    }
  }

  result
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let lines = content.split("\n");

  let mut forest: Vec<Vec<u32>> = vec![];
  for line in lines {
    forest.push(line.chars().map(|a| a.to_digit(10).expect("couldn't parse tree height")).collect::<Vec<u32>>());
  }

  let visibility = calc_visibility_score(&forest);

  // for f in &visibility {
  //   for t in f {
  //     //print!("{:?}", if t.4 {1} else {0} );
  //     print!("{:?}", t.4 );
  //   }
  //   println!();
  // }

  let vcount: u32 = visibility.iter().map(|a| a.iter().map(
    |h| if h.0 || h.1 || h.2 || h.3 {1} else {0}).sum::<u32>())
    .sum();
  
  let max_score: u32 = visibility.iter().map(|a| a.iter().map(
    |h| h.4).max().expect("failed finding max score"))
    .max().expect("failed finding max score");
  
    writeln!(stdout, "{vcount}").ok();
    writeln!(stdout, "{max_score}").ok();
}