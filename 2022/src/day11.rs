#[derive(Clone)]
struct Monkey {
  operation: String,
  arg: String,
  test: u32,
  if_true: usize,
  if_false: usize,

  inspect_count: u64
}

impl Monkey {
  fn do_operation(&self, w: u64) -> u64 {
    if self.arg == "old" {
      match self.operation.as_str() {
        "+" => w + w,
        "*" => w * w,
        _ => w
      }
    } else {
      let argi: u64 = self.arg.parse().expect("failed parsing arg");
      match self.operation.as_str() {
        "+" => w + argi,
        "*" => w * argi,
        _ => w
      }
    }
  }

  pub fn inspect(&mut self, worry: &u64, base: &u32) -> (usize, u64) {
    let mut new_worry = self.do_operation(*worry);
    if base == &0 {
      new_worry /= 3;
    } else {
      new_worry = new_worry % (*base as u64);
    }
    self.inspect_count += 1;
    let new_monkey = if (new_worry % (self.test as u64)) == 0 {self.if_true} else {self.if_false};
    (new_monkey, new_worry)
  }
}

fn parse_monkey(monkey_data: &str, items: &mut Vec<(usize, u64)>) -> Monkey {
  let lines: Vec<&str> = monkey_data.split("\n").collect();
  let monkey_index: usize = {
    let bind: Vec<&str> = lines[0].split(" ").collect();
    let sliced: &str = &bind[1][0..bind[1].len()-1];
    sliced.parse().expect(format!("failed parsing monkey_index {:?}", sliced).as_str())
  };
  let starting_items: Vec<u64> = {
    let bind: Vec<&str> = lines[1].trim().split(":").collect();
    bind[1].trim().split(", ").map(|i| i.parse().expect(format!("failed parsing starting item {i}").as_str())).collect()
  };
  let opline_bind: Vec<&str> = lines[2].trim().split(" ").collect();
  let op = opline_bind[4];
  let arg = opline_bind[5];

  for item in starting_items {
    items.push((monkey_index, item));
  }

  Monkey{
    operation: op.to_string(),
    arg: arg.to_string(),
    test: {
      lines[3].trim().split(" ").last().expect("couldn't get last").parse().expect("failed parsing test")
    },
    if_true: {
      lines[4].trim().split(" ").last().expect("couldn't get last").parse().expect("failed parsing if_true")
    },
    if_false: {
      lines[5].trim().split(" ").last().expect("couldn't get last").parse().expect("failed parsing if_false")
    },
    inspect_count: 0
  }
}

fn do_round(monkeys: &mut Vec<Monkey>, items: &mut Vec<(usize, u64)>, base: &u32) {
  for curr_monkey_index in 0..monkeys.len() {
    for (monkey_index, item) in items.iter_mut() {
      if *monkey_index != curr_monkey_index { continue; }
      (*monkey_index, *item) = monkeys[*monkey_index].inspect(item, base);
    }
  }
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let monkey_data = content.split("\n\n");
  
  let mut items: Vec<(usize, u64)> = vec![];
  let monkeys: Vec<Monkey> = monkey_data.map(|m| parse_monkey(m, &mut items)).collect();
  let base = monkeys.iter().map(|m| m.test).reduce(|accum, a| accum * a).expect("error calculating base");

  let mut monkey_business: u64;
  {
    let mut monkeys_p1 = monkeys.clone();
    let mut items_p1 = items.clone();
    for _round in 0..20 {
      do_round(&mut monkeys_p1, &mut items_p1, &0);
    }
    monkeys_p1.sort_by(|a,b| a.inspect_count.partial_cmp(&b.inspect_count).unwrap());
    monkey_business = monkeys_p1[monkeys_p1.len()-1].inspect_count * monkeys_p1[monkeys_p1.len()-2].inspect_count;
    writeln!(stdout, "{:?}", monkey_business).ok();
  }

  {
    let mut monkeys_p2 = monkeys.clone();
    let mut items_p2 = items.clone();
    for _round in 0..10000 {
      do_round(&mut monkeys_p2, &mut items_p2, &base);
    }
    monkeys_p2.sort_by(|a,b| a.inspect_count.partial_cmp(&b.inspect_count).unwrap());
    monkey_business = monkeys_p2[monkeys_p2.len()-1].inspect_count * monkeys_p2[monkeys_p2.len()-2].inspect_count;
    writeln!(stdout, "{:?}", monkey_business).ok();
  }
}