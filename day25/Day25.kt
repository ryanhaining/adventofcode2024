import java.util.Collections

const val MAX_PIN_DEPTH = 5
const val INPUT_PIECE_LEN = 8
const val MARK = "#####"

sealed interface Part {
  val pins: List<Int>
}

data class Key(override val pins: List<Int>): Part
data class Lock(override val pins: List<Int>): Part

fun countPins(lines: List<String>): List<Int> =
  (0..<lines.size).map { i ->
    lines.count { it[i] == '#' }
  }

fun toPart(lines: List<String>): Part {
  val pins = countPins(lines.drop(1).dropLast(1))
  return if (lines.first() == MARK) {
    Lock(pins)
  } else {
    require(lines.last() == MARK)
    Key(pins)
  }
}

fun main() {
  val parts = generateSequence(::readLine).chunked(INPUT_PIECE_LEN).map{
    // drop the blank line
    if (it.size == INPUT_PIECE_LEN) it.dropLast(1) else it
  }.map(::toPart)
  .toList()

  val keys = parts.filterIsInstance<Key>()
  val locks = parts.filterIsInstance<Lock>()

  val result = locks.map { lock ->
    keys.count { key ->
      lock.pins.zip(key.pins).all { (lp, kp) -> kp+lp <= MAX_PIN_DEPTH }
    }
  }.sum()

  println(result)
}
