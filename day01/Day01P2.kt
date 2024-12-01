import kotlin.math.abs

fun main() {
  val nums = mutableListOf<Int>()
  val counts = mutableMapOf<Int, Int>()
  for (line in generateSequence(::readLine)) {
    val (n1, n2) = line.split("""\s+""".toRegex()).map{ it.toInt() }
    nums.add(n1)
    counts[n2] = (counts[n2] ?: 0) + 1
  }
  val result = nums.map { it * (counts[it] ?: 0) }.sum()
  println(result)
}

