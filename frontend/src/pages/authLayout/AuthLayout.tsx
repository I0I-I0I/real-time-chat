import { Gradient } from "@/components/UI"
import { Outlet } from "react-router"
import { useUserStore } from "@/state/user"

const AuthLayout = () => {
    const setUser = useUserStore((state) => state.setUser)

	return (
		<>
			<Gradient />
            <Outlet context={setUser} />
		</>
	)
}

export default AuthLayout
