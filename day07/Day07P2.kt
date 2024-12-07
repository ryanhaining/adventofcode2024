fun hasSolution(ans: Long, soFar: Long, nums: List<Long>): Boolean {
  if (nums.isEmpty()) {
    return ans == soFar
  }
  val n = nums.first()
  val rest = nums.drop(1)
  return hasSolution(ans, soFar * n, rest) || hasSolution(ans, soFar + n, rest) || hasSolution(ans, "$soFar$n".toLong(), rest)
}

fun lineVal(line: String): Long {
    val (ansStr, numStrs) = line.split(":")
    val ans = ansStr.toLong()
    val nums = numStrs.trim().split(" ").map { it.toLong() }
    return if (hasSolution(ans, nums.first(), nums.drop(1))) {
      ans
    } else {
      0
    }
}

fun main() {
  println(generateSequence(::readLine).map(::lineVal).sum())
}
