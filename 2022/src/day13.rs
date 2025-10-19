use std::cmp::Ordering;
use std::fmt;

fn check_order(l: &Data, r: &Data, data: &Vec<Data>) -> Ordering {
  // println!("left={:?}; right={:?}; ",l,r);

  for i in 0..l.d.len() {
    if i >= r.d.len() { 
      // println!("return Greater due to len"); 
      return Ordering::Greater; 
    }

    let result = match (l.d[i], r.d[i]) {
      ((None,Some(left_int)),(None,Some(right_int))) => {
        left_int.cmp(&right_int)
      },
      ((Some(left_ind),None),(None,Some(right_int))) => {
        let right_data = Data{ d:vec![(None, Some(right_int));1] };
        // println!("mixed types, converting right to [{right_int}] and retrying");
        check_order(&data[left_ind], &right_data, data)
      },
      ((None,Some(left_int)),(Some(right_ind),None)) => {
        let left_data = Data{ d:vec![(None, Some(left_int));1] };
        // println!("mixed types, converting left to [{left_int}] and retrying");
        check_order(&left_data, &data[right_ind], data)
      },
      ((Some(left_ind),None),(Some(right_ind),None)) => {
        check_order(&data[left_ind], &data[right_ind], data)
      },
      _ => Ordering::Less
    };
    if result != Ordering::Equal { 
      // println!("return {:?}",result); 
      return result; 
    }
  }

  if l.d.len() < r.d.len() { 
    // println!("return Less due to len"); 
    return Ordering::Less; 
  }

  // println!("default return Equal");
  Ordering::Equal
}

struct Data {
  d: Vec<(Option<usize>, Option<u32>)>
}

impl fmt::Debug for Data {
  fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
    for (ind, dat) in self.d.iter().enumerate() {
      if ind!=0 { write!(f, ",").expect(""); }
      let _ = match dat {
        (None, Some(i)) => write!(f, "{i}"),
        (Some(i), None) => write!(f, "[*{i}]"),
        _ => write!(f, "")
      };
    }
    write!(f, "")
  }
}

fn parse_list(input: &str, data: &mut Vec<Data>) -> usize {
  let mut packet_data = Data{d: vec![]};
  let in_stripped = &input[1..input.len()-1];
  let mut list_depth = 0;
  let mut p = 0; // index of the beginning of this data
  // println!("parse_list({in_stripped})");
  for (i, c) in in_stripped.chars().enumerate() { 
    if c == '[' {
      list_depth += 1;
    } else if c == ']' {
      list_depth -= 1;
    }
    if c == ',' || i == (in_stripped.len()-1) {
      if list_depth == 0 {
        let e = if i == (in_stripped.len()-1) {i+1} else {i};
        let s = &in_stripped[p..e];
        // println!("{p}..{e}: {s}");
        packet_data.d.push(if s.starts_with('[') {
          (Some(parse_list(s, data)), None)
        } else {
          (None, Some(s.parse().expect("failed parsing")))
        });
        p = i+1;
      }
    }
  }

  let index = data.len();
  // println!("{index}: {:?}",packet_data);
  data.push(packet_data);
  index
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let pair_lines_list : Vec<&str> = content.split("\n\n").collect();

  let mut data = Vec::<Data>::new();
  let mut pairs = Vec::<(usize,usize)>::new();
  for pair_lines in pair_lines_list {
    let lines: Vec<&str> = pair_lines.split("\n").collect();
    let p1 = parse_list(lines[0], &mut data);
    let p2 = parse_list(lines[1], &mut data);
    // println!("pairs[{:?}] = ({p1}, {p2})",pairs.len());
    pairs.push((p1,p2));
  }

  let mut sum = 0;
  for (i, (l,r)) in pairs.iter().enumerate() {
    sum += if check_order(&data[*l], &data[*r], &data)==Ordering::Less {i+1} else {0};
  }

  writeln!(stdout, "{sum}").ok();

  let d1 = parse_list("[[2]]", &mut data);
  let d2 = parse_list("[[6]]", &mut data);

  let mut all_packet_indices = Vec::<usize>::new();
  for p in pairs.iter(){
    all_packet_indices.push(p.0);
    all_packet_indices.push(p.1);
  }
  all_packet_indices.push(d1);
  all_packet_indices.push(d2);

  all_packet_indices.sort_by(|a,b| check_order(&data[*a], &data[*b],&data));

  let d1_ind = all_packet_indices.iter().position(|a| a==&d1).expect("d1 not found");
  let d2_ind = all_packet_indices.iter().position(|a| a==&d2).expect("d2 not found");
  let product = (d1_ind+1) * (d2_ind+1);
  writeln!(stdout, "{product}").ok();

}