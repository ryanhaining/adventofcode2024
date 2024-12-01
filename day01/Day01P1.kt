import kotlin.math.abs

fun main() {
  val left = mutableListOf<Int>()
  val right = mutableListOf<Int>()
  for (line in generateSequence(::readLine)) {
    val (n1, n2) = line.split("""\s+""".toRegex()).map{ it.toInt() }
    left.add(n1)
    right.add(n2)
  }
  left.sort()
  right.sort()
  val result = left.zip(right).map { (n1, n2) -> abs(n1-n2) }.sum()
  println(result)
}

