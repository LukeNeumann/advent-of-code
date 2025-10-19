
struct Dir {
  name: String,
  files: Vec<(u32,String)>,
  dirs: Vec<usize>,
  parent: usize
}

fn get_dir_size(index: usize, fs : &Vec<Dir>) -> u32 {
  fs[index].files.iter().map(|a| a.0).sum::<u32>() +
  fs[index].dirs.iter().map(|&a| get_dir_size(a, fs)).sum::<u32>()
}


fn parse_terminal(commands : Vec<&str>, fs : &mut Vec<Dir>) {
  let mut cwd = 0;

  for whole_command in commands {
    let lines : Vec<&str> = whole_command.trim().split("\n").collect();
    let cmd: Vec<&str> = lines[0].split(" ").collect();
    let cmd_type = cmd[0];
    let cmd_arg = if cmd.len() > 1 { cmd[1] } else { "" };
    let cmd_output = &lines[1..];

    let _r = match cmd_type {

      "cd" => {
        let result = match cmd_arg {
          "" => { Err("error: cd command needs arg") },
          ".." => {
            cwd = fs[cwd].parent;
            Ok(cmd_type)
          },
          "/" => { 
            cwd = 0; 
            Ok(cmd_type) 
          },
          _ => { 
            cwd = *fs[cwd].dirs.iter().find(|&&d| fs[d].name == cmd_arg).expect("can't cd"); 
            Ok(cmd_type)
          }
        };
        // println!("{cmd_type} {cmd_arg}; cwd = {cwd}");
        result
      },

      "ls" => {
        // println!("{cmd_type}; cwd = {cwd}");
        for line in cmd_output {
          let info: Vec<&str> = line.split(" ").collect();
          if info[0] == "dir" {
            let new_dir_index = fs.len();
            fs[cwd].dirs.push(new_dir_index);
            // println!("new dir {new_dir_index} {:?}", info[1]);
            fs.push(Dir { name: String::from(info[1]), files: vec![], dirs: vec![], parent: cwd });
          } else {
            let file_size : u32 = info[0].parse().expect("couldn't read size");
            fs[cwd].files.push((file_size,String::from(info[1])));
          }
        }
        Ok(cmd_type)
      },

      _ => Err("invalid command")

    };    
  }
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let commands : Vec<&str> = content.split("$ ").collect();
  
  let root = Dir { name: String::from("/"), files: vec![], dirs: vec![], parent: 0 };
  let mut fs = vec![root];
  parse_terminal(commands, &mut fs);

  let mut sum = 0;
  let min_size_to_free = 30000000 - (70000000 - get_dir_size(0, &fs));
  let mut size_to_free = 70000000;
  for (index,_dir) in fs.iter().enumerate() {
    let size = get_dir_size(index, &fs);
    // println!("{index} {:?}: {size}", _dir.name);
    if size <= 100000 { sum += size; }

    if size < size_to_free && size >= min_size_to_free {
      size_to_free = size;
    }
  }

  writeln!(stdout, "{sum}").ok();
  writeln!(stdout, "{size_to_free}").ok();
}