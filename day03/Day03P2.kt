private val MUL = """mul\((\d+),(\d+)\)""".toRegex()
private val DO_NOT = """don't\(\).*?(do\(\)|$)""".toRegex()

fun main() {
  val contents = generateSequence(::readLine).joinToString("")
  println(
      MUL.findAll(contents.replace(DO_NOT, ""))
      .map { it.groupValues.drop(1).map(String::toInt) }
      .map { (n1, n2) -> n1 * n2 }
      .sum()
  )
}
