use std::collections::HashMap;

fn detect(bytes : &[u8], num : usize) -> usize {
  let mut recents = HashMap::<u8,u32>::new();

  for i in 0..bytes.len() {
    let c = bytes[i];
    match recents.get(&c) {
      Some(count) => { recents.insert(c, count + 1); },
      None => { recents.insert(c, 1); }
    }
    
    if i >= num {
      let old_c = bytes[i - num];
      match recents.get(&old_c) {
        Some(1) => { recents.remove(&old_c); }
        Some(count) => { recents.insert(old_c, count - 1); },
        None => { println!("error: couldn't remove old character {old_c}"); }
      }
    }

    //println!("{i}: {:?}", recents);

    if recents.keys().len() == num {
      return i + 1;
    }
  }

  0
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let chars = content.trim().as_bytes();
  

  writeln!(stdout, "{:?}", detect(chars, 4)).ok();
  writeln!(stdout, "{:?}", detect(chars, 14)).ok();
}