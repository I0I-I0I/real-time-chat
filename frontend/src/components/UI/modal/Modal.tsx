import cls from "@/utils/cls"
import styles from "./Modal.module.css"

interface ModalProps {
	children: React.ReactNode
	className?: string
}

export const Modal = ({
	children,
	className = ""
}: ModalProps) => (
	<div className={cls(styles.modal, className)}>
		{children}
	</div>
)

