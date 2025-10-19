#![allow(unused_parens)]

mod day1;
mod day2;
mod day3;
mod day4;
mod day5;
mod day6;
mod day7;
mod day8;
mod day9;
mod day10;
mod day11;
mod day12;
mod day13;
mod day14;
mod day15;
mod day16;
mod day17;
mod day18;
mod day19;
mod day20;
mod day21;
mod day22;
mod day23;
mod day24;
mod day25;


fn run_day(stdout: &mut dyn std::io::Write, day : &str, ex : &str) {
    let under = if ex == "" {""} else {"_"};
    let binding = std::fs::read_to_string(format!("input/{day}{under}{ex}.txt")).unwrap();
    let content = binding.trim();
    match day {
        "1" => day1::run(stdout, &content),
        "2" => day2::run(stdout, &content),
        "3" => day3::run(stdout, &content),
        "4" => day4::run(stdout, &content),
        "5" => day5::run(stdout, &content),
        "6" => day6::run(stdout, &content),
        "7" => day7::run(stdout, &content),
        "8" => day8::run(stdout, &content),
        "9" => day9::run(stdout, &content),
        "10" => day10::run(stdout, &content),
        "11" => day11::run(stdout, &content),
        "12" => day12::run(stdout, &content),
        "13" => day13::run(stdout, &content),
        "14" => day14::run(stdout, &content),
        "15" => day15::run(stdout, &content),
        "16" => day16::run(stdout, &content),
        "17" => day17::run(stdout, &content),
        "18" => day18::run(stdout, &content),
        "19" => day19::run(stdout, &content),
        "20" => day20::run(stdout, &content),
        "21" => day21::run(stdout, &content),
        "22" => day22::run(stdout, &content),
        "23" => day23::run(stdout, &content),
        "24" => day24::run(stdout, &content),
        "25" => day25::run(stdout, &content),
        _ => {}
    }
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let mut day = "1";
    if args.len() > 1 { day = &args[1]; }
    let mut ex = "";
    if args.len() > 2 { ex = &args[2]; }
    run_day(&mut std::io::stdout(), day, &ex);
}

#[test]
fn test_day1() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "1", "");
    assert_eq!(stdout, b"70698\n206643\n");
}
#[test]
fn test_day2() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "2", "");
    assert_eq!(stdout, b"11873\n12014\n");
}
#[test]
fn test_day3() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "3", "");
    assert_eq!(stdout, b"7889\n2825\n");
}
#[test]
fn test_day4() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "4", "");
    assert_eq!(stdout, b"459\n779\n");
}
#[test]
fn test_day5() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "5", "");
    assert_eq!(stdout, b"QNHWJVJZW\nBPCZJLFJW\n");
}
#[test]
fn test_day6() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "6", "");
    assert_eq!(stdout, b"1100\n2421\n");
}
#[test]
fn test_day7() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "7", "");
    assert_eq!(stdout, b"2031851\n2568781\n");
}
#[test]
fn test_day8() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "8", "");
    assert_eq!(stdout, b"1688\n410400\n");
}
#[test]
fn test_day9() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "9", "");
    assert_eq!(stdout, b"6354\n2651\n");
}
#[test]
fn test_day10() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "10", "");
    assert_eq!(stdout, b"17940\n####..##..###...##....##.####...##.####.\n...#.#..#.#..#.#..#....#.#.......#....#.\n..#..#....###..#..#....#.###.....#...#..\n.#...#....#..#.####....#.#.......#..#...\n#....#..#.#..#.#..#.#..#.#....#..#.#....\n####..##..###..#..#..##..#.....##..####.\n");
}
#[test]
fn test_day11() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "11", "");
    assert_eq!(stdout, b"55930\n14636993466\n");
}
#[test]
fn test_day12() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "12", "");
    assert_eq!(stdout, b"350\n349\n");
}
#[test]
fn test_day13() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "13", "");
    assert_eq!(stdout, b"6086\n27930\n");
}
#[test]
fn test_day14() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "14", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day15() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "15", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day16() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "16", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day17() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "17", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day18() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "18", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day19() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "19", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day20() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "20", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day21() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "21", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day22() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "22", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day23() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "23", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day24() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "24", "");
    assert_eq!(stdout, b"not implemented\n");
}
#[test]
fn test_day25() {
    let mut stdout = Vec::new();
    run_day(&mut stdout, "25", "");
    assert_eq!(stdout, b"not implemented\n");
}

