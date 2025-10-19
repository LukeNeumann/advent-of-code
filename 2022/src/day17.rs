pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let _lines : Vec<&str> = content.split("\n").collect();
  
  writeln!(stdout, "not implemented").ok();
}