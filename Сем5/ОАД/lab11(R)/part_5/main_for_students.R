library(neuralnet)

# === Данные: учим складывать два числа от 0 до 20 ===
set.seed(42)
mydata <- data.frame(
  input_1 = sample(0:20, 50, replace = TRUE),
  input_2 = sample(0:20, 50, replace = TRUE)
)
mydata$output <- mydata$input_1 + mydata$input_2

# === Нормализация в [0, 1] ===
max_data <- c(20, 20, 40)   # максимумы по колонкам
min_data <- c(0,  0,  0)

scaled <- as.data.frame(lapply(1:3, function(i) {
  (mydata[[i]] - min_data[i]) / (max_data[i] - min_data[i])
}))
names(scaled) <- c("input_1", "input_2", "output")

# === Обучаем сеть — теперь всё идеально сходится ===
nn <- neuralnet(
  output ~ input_1 + input_2,
  data = scaled,
  hidden = c(8, 8),          # два слоя по 8 нейронов — достаточно
  threshold = 0.001,         # строже
  stepmax = 1e6,
  rep = 5,                   # 5 повторов — берём лучший
  learningrate = 0.1,
  algorithm = "rprop+",      # лучший алгоритм для таких задач
  linear.output = TRUE,      # обязательно для регрессии
  lifesign = "minimal"
)
quartz()
# Берём лучший результат
best <- which.min(nn$result.matrix[1, ])
plot(nn, rep = best, show.weights = FALSE, information = TRUE, fontsize = 12, dimension = c(10,8))
dev.copy(png, "neural_network.png", width = 1200, height = 800); dev.off()
# === Предсказание + обратная нормализация ===
pred <- predict(nn, scaled[,1:2], rep = best)
pred_original <- pred * 40   # потому что максимум суммы = 40

# === Красивая таблица ===
result <- data.frame(
  A = mydata$input_1,
  B = mydata$input_2,
  Ожидаем = mydata$output,
  Предсказание = round(pred_original, 4),
  Ошибка = round(mydata$output - pred_original, 6)
)

print(result)

# Последние 10 строк — проверь, всё ли идеально
tail(result, 10)
