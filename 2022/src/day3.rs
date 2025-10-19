use std::collections::HashMap;

fn find_shared_item(rucksack : Vec<char>) -> char {
    let mut items_c1 : HashMap<char, bool> = HashMap::new();
    let mut items_c2 : HashMap<char, bool> = HashMap::new();

    for i in 0..(rucksack.len()/2) {
        if(items_c2.contains_key(&rucksack[i])) { return rucksack[i]; }
        items_c1.insert(rucksack[i], true);
        let j = rucksack.len()-1 - i;
        if(items_c1.contains_key(&rucksack[j])) { return rucksack[j]; }
        items_c2.insert(rucksack[j], true);
    }

    return 'a';
}

fn calculate_priority(a : &char) -> u32 {
    return (if a.is_ascii_uppercase() {27} else {1}) + (a.to_ascii_lowercase() as u32 - 'a' as u32);
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let lines : Vec<&str> = content.split("\n").collect();

  let sum1 = lines.iter().map(|a| { find_shared_item(a.chars().collect()) } )
  .map(|a| calculate_priority(&a))
  .sum::<u32>();

  let mut sum2 = 0;
  let mut b : HashMap<char, u32> = HashMap::new(); // map of potential badges
  for (index, line) in lines.iter().enumerate() {
    let im3 = (index as u32) % 3;
    if im3 == 0 { b = HashMap::new(); }

    for c in line.chars() {
        if(im3 == 0 || b.get(&c) == Some(&im3)) {
            b.insert(c, im3 + 1);
            if(im3 == 2) { sum2 += calculate_priority(&c); }
        }
    }
  }

  writeln!(stdout, "{:?}", sum1).ok();
  writeln!(stdout, "{:?}", sum2).ok();
}