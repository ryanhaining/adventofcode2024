private val MUL = """mul\((\d+),(\d+)\)""".toRegex()

fun main() {
  val contents = generateSequence(::readLine).joinToString("")
  println(
      MUL.findAll(contents)
      .map { it.groupValues.drop(1).map(String::toInt) }
      .map { (n1, n2) -> n1 * n2 }
      .sum()
  )
}
