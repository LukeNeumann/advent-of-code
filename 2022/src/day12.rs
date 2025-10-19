use petgraph::graph::Graph;
use petgraph::graph::NodeIndex;
use petgraph::algo::astar;
use std::collections::HashMap;

fn add_edge(x1: usize, y1: usize, x2: usize, y2: usize, node_coords: &HashMap::<(usize,usize), NodeIndex>, g: &mut Graph::<char,u32>, p2: bool) {
  let index1 = *node_coords.get(&(x1,y1)).expect("no_node1");
  let index2 = *node_coords.get(&(x2,y2)).expect("no_node2");
  let cond = if(p2) {
    (g[index1].to_ascii_lowercase() as u32) <= (g[index2].to_ascii_lowercase() as u32 + 1)
  } else {
    (g[index1].to_ascii_lowercase() as u32) >= (g[index2].to_ascii_lowercase() as u32 - 1)
  };
  if cond {
    //println!("{x1} {y1} {:?}; {x2} {y2} {:?};", g[index1], g[index2]);
    g.add_edge(index1, index2, 1);
  }
}

fn add_edges(nrows: usize, ncols: usize, node_coords: &HashMap::<(usize,usize), NodeIndex>, g: &mut Graph::<char,u32>, p2: bool) {
  for y in 0..nrows {
    for x in 0..ncols {
      if y > 0       { add_edge(x,y,x,y-1, &node_coords, g, p2); }
      if y < nrows-1 { add_edge(x,y,x,y+1, &node_coords, g, p2); }
      if x > 0       { add_edge(x,y,x-1,y, &node_coords, g, p2); }
      if x < ncols-1 { add_edge(x,y,x+1,y, &node_coords, g, p2); }
    }
  }
}

pub fn run(stdout: &mut dyn std::io::Write, content : &str) {
  let lines: Vec<&str> = content.split("\n").collect();

  let mut start: Option<NodeIndex> = None;
  let mut target: Option<NodeIndex> = None;
  let mut g = Graph::<char,u32>::new();
  let mut node_coords = HashMap::<(usize,usize), NodeIndex>::new();

  for (y,line) in lines.iter().enumerate() {
    for (x,c) in line.chars().enumerate() {
      let h = match c {
        'S' => {'a'},
        'E' => {'z'},
        h => { h },
      };
      let cur = g.add_node(h);
      if c == 'S' { start = Some(cur); }
      if c == 'E' { target = Some(cur); }
      node_coords.insert((x,y), cur);
    }
  }

  let mut g2 = g.clone();

  add_edges(lines.len(), lines[0].len(), &node_coords, &mut g, false);
  let path = astar(&g, start.expect("s"), |n| n == target.expect("t"), |e| *e.weight(), |_| 0);  
  let min_steps = path.expect("no path!").0;
  writeln!(stdout, "{min_steps}").ok();

  add_edges(lines.len(), lines[0].len(), &node_coords, &mut g2, true);
  let path2 = astar(&g2, target.expect("t"), |n| g2[n] == 'a', |e| *e.weight(), |_| 0); 
  let min_steps2 = path2.expect("no path!").0;
  writeln!(stdout, "{min_steps2}").ok();
}