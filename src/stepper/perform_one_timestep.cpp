hpx::future<void> partition::perform_one_time_step(int t){

  // Call get on the channel (left_from and right_from)
  // which produces a future
  hpx::future<void> left_boundary_future = channel_left_from.get(t);
  hpx::future<void> right_boundary_future = channel_right_from.get(t);

  // How do I get current state?


  // question: if it's an hpx::future<void>, why does it return a std::vector<double>?

  // Call .get on the incoming futures
  std::vector<double> data = channel_left_from.get();
  std::vector<double> data = channel_right_from.get();

  return when_all(left_boundary_future, right_boundary_future, interior_future);
}
